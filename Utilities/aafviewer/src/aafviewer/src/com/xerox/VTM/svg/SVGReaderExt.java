package com.xerox.VTM.svg;


import java.util.*;
import java.awt.Font;

import com.xerox.VTM.engine.*;
import com.xerox.VTM.glyphs.*;

import org.w3c.dom.Element;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;

public class SVGReaderExt extends SVGReader {

    private Vector _listeners;
    private String _title; 


    public SVGReaderExt() {
	_listeners = new Vector();
    }

    public void registerListener( SVGReaderExtListener listener ) {
	_listeners.add( listener );
    }

    // load and process the SVG document
    public void loadExt( Document d, VirtualSpaceManager vsm, String vs ) {
	Element svgRoot = d.getDocumentElement();
	NodeList objects = svgRoot.getChildNodes();
	for ( int i=0; i<objects.getLength(); i++ ) {
	    Node obj = objects.item( i );
	    if ( obj.getNodeType()==Node.ELEMENT_NODE ) {
		processNodeExt( ( Element )obj, vsm, vs, null, false);
	    }
	}
    }

    // report the creation of a Glyph to the listeners
    private void reportGlyphCreated( Glyph glyph ) {
	Iterator iter = _listeners.iterator();
	while ( iter.hasNext() ) {
	    SVGReaderExtListener listener = (SVGReaderExtListener)iter.next();
	    listener.glyphCreated( glyph, _title );
	}
    }

    // process a DOM node in the SVG document
    private void processNodeExt( Element e, VirtualSpaceManager vsm, String vs, Context ctx, boolean mainFontSet ) {
	String tagName = e.getTagName();
	if ( tagName.equals( _rect ) ) {
	    Glyph glyph = createRectangle( e, ctx );
	    vsm.addGlyph( glyph, vs );
	    reportGlyphCreated( glyph );
	}
	else if ( tagName.equals( _ellipse ) ) {
	    Glyph glyph = createEllipse( e, ctx );
	    vsm.addGlyph( glyph, vs );
	    reportGlyphCreated( glyph );
	}
	else if ( tagName.equals( _circle) ) {
	    Glyph glyph = createCircle( e, ctx );
	    vsm.addGlyph( glyph, vs );
	    reportGlyphCreated( glyph );
	}
	else if ( tagName.equals( _path ) ) {
	    Glyph glyph = createPath( e, new VPath(), ctx );
	    vsm.addGlyph( glyph, vs );
	    reportGlyphCreated( glyph );
	}
	else if ( tagName.equals( _text ) ) {
	    Glyph glyph = createText( e, ctx, vsm );
	    vsm.addGlyph( glyph, vs );
	    reportGlyphCreated( glyph );
	}
	else if ( tagName.equals( _polygon ) ) {
	    Glyph glyph = createRectangleFromPolygon( e, ctx );
	    if ( glyph != null ) {
		vsm.addGlyph( glyph, vs ); 
		reportGlyphCreated( glyph );
	    }
	    else {
		glyph = createPolygon( e, ctx );
		vsm.addGlyph( glyph, vs );
		reportGlyphCreated( glyph );
	    }
	}
	else if ( tagName.equals( _polyline ) ) {
	    Glyph[] segments = createPolyline( e, ctx );
	    for ( int i=0; i<segments.length; i++ ) {
		vsm.addGlyph( segments[i], vs );
		reportGlyphCreated( segments[i] );
	    }
	}
	else if ( tagName.equals( _g ) ) {
	    NodeList objects = e.getChildNodes();
	    boolean setAFont = false;
	    if ( e.hasAttribute( SVGReader._style ) ) {
		if ( ctx != null ) { ctx.add( e.getAttribute( SVGReader._style ) ); }
		else { ctx = new Context( e.getAttribute( SVGReader._style ) ); }
		if ( !mainFontSet ) {
		    Font f;
		    if ( ( f = ctx.getDefinedFont() ) !=null ) {
			vsm.setMainFont( f );
			setAFont = true;
		    }
		}
		else { setAFont = true; }
	    }
	    for ( int i=0;i<objects.getLength( );i++ ) {
		Node obj=objects.item( i );
		if ( obj.getNodeType()==Node.ELEMENT_NODE ) { processNodeExt( ( Element )obj,vsm,vs,ctx,setAFont );}
	    }
	}
	else if ( tagName.equals( _a ) ) {
	    NodeList objects=e.getChildNodes();
	    boolean setAFont=false;
	    if ( e.hasAttribute( SVGReader._style ) ) {
		if ( ctx!=null ) { ctx.add( e.getAttribute( SVGReader._style ) ); }
		else { ctx=new Context( e.getAttribute( SVGReader._style ) ); }
		if ( !mainFontSet ) {
		    Font f;
		    if ( ( f=ctx.getDefinedFont() )!=null ) {
			vsm.setMainFont( f );
			setAFont=true;
		    }
		}
		else { setAFont=true; }
	    }
	    for ( int i=0;i<objects.getLength();i++ ) {
		Node obj=objects.item( i );
		if ( obj.getNodeType()==Node.ELEMENT_NODE ) { processNodeExt( ( Element )obj,vsm,vs,ctx,setAFont ); }
	    }
	}
	else if ( tagName.equals( "title" ) ) {
	    boolean done = false;
	    NodeList titleObjects = e.getChildNodes();
	    for ( int i=0; i<titleObjects.getLength() && !done; i++ ) {
		Node obj=titleObjects.item( i );
		if ( obj.getNodeType()==Node.TEXT_NODE ) {
		    _title = obj.getNodeValue();
		    done = true;
		}
	    }
	}
	else System.err.println( "SVGReader: unsupported element: "+tagName );
    }

    
}
