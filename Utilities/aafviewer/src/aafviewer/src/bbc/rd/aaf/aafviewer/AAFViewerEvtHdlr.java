/*   FILE: ZgrvEvtHdlr.java
 *   DATE OF CREATION:   Thu Jan 09 15:18:48 2003
 *   AUTHOR :            Emmanuel Pietriga (emmanuel@w3.org)
 *   MODIF:              Mon Feb 03 10:45:39 2003 by Emmanuel Pietriga
 *   Copyright (c) Emmanuel Pietriga, 2002. All Rights Reserved
 *   Licensed under the GNU LGPL. For full terms see the file COPYING.
 */

package bbc.rd.aaf.aafviewer;

import java.util.Iterator;

import java.util.Vector;
import java.awt.*;
import java.awt.event.KeyEvent;
import com.xerox.VTM.engine.*;
import com.xerox.VTM.glyphs.*;
import net.claribole.zvtm.engine.Location;

public class AAFViewerEvtHdlr extends AppEventHandler {

    AAFViewer application;

    long lastX,lastY,lastJPX,lastJPY;    //remember last mouse coords to compute translation  (dragging)
    float tfactor;
    float cfactor=20.0f;                 // zoom sensitivity factor
    float cmfactor=10.0f;                // movement sensitivity factor
    long x1,y1,x2,y2;                     //remember last mouse coords to display selection rectangle (dragging)

    private class SelectedEdge {
      public AAFViewer.GlyphTitle glyphTitle;
      public Color color;
    }
    private Vector _selectedEdges; // remember selected edges and the original colour


    VSegment navSeg;

    Camera activeCam;

    DisplayMode oldDM;

    boolean zoomingInRegion=false;
    boolean manualLeftButtonMove=false;
    boolean manualRightButtonMove=false;

    AAFViewerEvtHdlr(AAFViewer app){
	this.application=app;
	_selectedEdges = new Vector();
    }

    public void press1(ViewPanel v,int mod,int jpx,int jpy){
	application.rememberLocation(v.cams[0].getLocation());
	if (mod==NO_MODIFIER || mod==SHIFT_MOD){
	    manualLeftButtonMove=true;
	    lastJPX=jpx;
	    lastJPY=jpy;
	    //AAFViewer.vsm.setActiveCamera(v.cams[0]);
	    v.setDrawDrag(true);
	    AAFViewer.vsm.activeView.mouse.setSensitivity(false);  //because we would not be consistent  (when dragging the mouse, we computeMouseOverList, but if there is an anim triggered by {X,Y,A}speed, and if the mouse is not moving, this list is not computed - so here we choose to disable this computation when dragging the mouse with button 3 pressed)
	    activeCam=application.vsm.getActiveCamera();
	}
	else if (mod==CTRL_MOD){
	    zoomingInRegion=true;
	    x1=v.getMouse().vx;
	    y1=v.getMouse().vy;
	    v.setDrawRect(true);
	}
    }

    public void release1(ViewPanel v,int mod,int jpx,int jpy){
	if (zoomingInRegion){
	    v.setDrawRect(false);
	    x2=v.getMouse().vx;
	    y2=v.getMouse().vy;
	    if ((Math.abs(x2-x1)>=4) && (Math.abs(y2-y1)>=4)){
		AAFViewer.vsm.centerOnRegion(AAFViewer.vsm.getActiveCamera(),ConfigManager.ANIM_MOVE_LENGTH,x1,y1,x2,y2);
	    }
	    zoomingInRegion=false;
	}
	else if (manualLeftButtonMove){
	    AAFViewer.vsm.animator.Xspeed=0;
	    AAFViewer.vsm.animator.Yspeed=0;
	    AAFViewer.vsm.animator.Aspeed=0;
	    v.setDrawDrag(false);
	    AAFViewer.vsm.activeView.mouse.setSensitivity(true);
	    manualLeftButtonMove=false;
	}
    }

    public void click1(ViewPanel v,int mod,int jpx,int jpy,int clickNumber){
      if ( mod == AppEventHandler.NO_MODIFIER ||
          mod != AppEventHandler.SHIFT_MOD ) {
	  // toggle select/de-select of OUTGOING object references

          Glyph glyph = v.lastGlyphEntered();
          if ( glyph != null ) {
	      // find the matching glyph
	      Iterator glyphIter =
                  application.getGlyphTitles().iterator();
              boolean found = false;
	      boolean done = false;
	      AAFViewer.GlyphTitle glyphTitle = null;
              while ( glyphIter.hasNext() && !done ) {
                  glyphTitle = (AAFViewer.GlyphTitle)glyphIter.next();
		  if ( glyph.equals( glyphTitle.glyph ) ) {
		      done = true;
		      if ( glyphTitle.isNode() ) {
			  found = true;
		      }
		  }
	      }

	      // find the outgoing edges for the node just found
	      // if node title is before the arrow then edge is outgoing
	      // eg node title = N1; edge title = N1 -> N2
              Vector foundGlyphs = new Vector();
	      if ( found ) {
		  glyphIter = application.getGlyphTitles().iterator();
		  found = false;
		  AAFViewer.GlyphTitle edgeGlyphTitle;
		  while ( glyphIter.hasNext() ) {
		      edgeGlyphTitle = (AAFViewer.GlyphTitle)glyphIter.next();
		      if ( edgeGlyphTitle.hasEdgeSource( glyphTitle.title ) ) {
			  foundGlyphs.add( edgeGlyphTitle );
		      }
                  }
              }

	      // if the edge is in the selected edge vector, then de-select; else select
	      glyphIter = foundGlyphs.iterator();
	      while ( glyphIter.hasNext() ) {
                  AAFViewer.GlyphTitle gt = (AAFViewer.GlyphTitle)glyphIter.next();
                  found = false;
		  Iterator selIter = _selectedEdges.iterator();
		  while ( !found && selIter.hasNext() ) {
                    SelectedEdge selEdge = (SelectedEdge)selIter.next();
                    if ( gt.glyph.equals( selEdge.glyphTitle.glyph ) )
                    {
                      found = true;
		      setGlyphColor( gt.glyph, selEdge.color );
                      selIter.remove();
                    }
                  }
                  if ( !found ) {
                    SelectedEdge selEdge = new SelectedEdge();
                    selEdge.glyphTitle = gt;
                    selEdge.color = gt.glyph.getColor();
                    _selectedEdges.add( selEdge );
                    setGlyphColor( gt.glyph, Color.red );
                  }
              }
          }
      }
      else if ( mod == AppEventHandler.SHIFT_MOD ) {
	  // toggle select/de-select of INCOMING object references

          Glyph glyph = v.lastGlyphEntered();
          if ( glyph != null ) {
	      // find the matching glyph
	      Iterator glyphIter =
                  application.getGlyphTitles().iterator();
              boolean found = false;
	      boolean done = false;
	      AAFViewer.GlyphTitle glyphTitle = null;
              while ( glyphIter.hasNext() && !done ) {
                  glyphTitle = (AAFViewer.GlyphTitle)glyphIter.next();
		  if ( glyph.equals( glyphTitle.glyph ) ) {
		      done = true;
		      if ( glyphTitle.isNode() ) {
			  found = true;
		      }
		  }
	      }

	      // find the incoming edges for the node just found
	      // if node title is before the arrow then edge is incoming
	      // eg node title = N1; edge title = N2 -> N1
              Vector foundGlyphs = new Vector();
	      if ( found ) {
		  glyphIter = application.getGlyphTitles().iterator();
		  found = false;
		  AAFViewer.GlyphTitle edgeGlyphTitle;
		  while ( glyphIter.hasNext() ) {
		      edgeGlyphTitle = (AAFViewer.GlyphTitle)glyphIter.next();
		      if ( edgeGlyphTitle.hasEdgeTarget( glyphTitle.title ) ) {
			  foundGlyphs.add( edgeGlyphTitle );
		      }
                  }
              }

	      // if the edge is in the selected edge vector, then de-select; else select
	      glyphIter = foundGlyphs.iterator();
	      while ( glyphIter.hasNext() ) {
                  AAFViewer.GlyphTitle gt = (AAFViewer.GlyphTitle)glyphIter.next();
                  found = false;
		  Iterator selIter = _selectedEdges.iterator();
		  while ( !found && selIter.hasNext() ) {
                    SelectedEdge selEdge = (SelectedEdge)selIter.next();
                    if ( gt.glyph.equals( selEdge.glyphTitle.glyph ) )
                    {
                      found = true;
		      setGlyphColor( gt.glyph, selEdge.color );
                      selIter.remove();
                    }
                  }
                  if ( !found ) {
                    SelectedEdge selEdge = new SelectedEdge();
                    selEdge.glyphTitle = gt;
                    selEdge.color = gt.glyph.getColor();
                    _selectedEdges.add( selEdge );
                    setGlyphColor( gt.glyph, Color.red );
                  }
              }
          }
      }
    }

    private void setGlyphColor( Glyph glyph, Color color ) {
	float hsv[] = Color.RGBtoHSB( color.getRed(), color.getGreen(), 
				      color.getBlue(), null );
	glyph.setHSVColor( hsv[0], hsv[1], hsv[2] );
    }

    public void press2(ViewPanel v,int mod,int jpx,int jpy){}
    public void release2(ViewPanel v,int mod,int jpx,int jpy){}
    public void click2(ViewPanel v,int mod,int jpx,int jpy,int clickNumber){}

    public void press3(ViewPanel v,int mod,int jpx,int jpy){
	application.rememberLocation(v.cams[0].getLocation());
	lastJPX=jpx;
	lastJPY=jpy;
	//AAFViewer.vsm.setActiveCamera(v.cams[0]);
	v.setDrawDrag(true);
	AAFViewer.vsm.activeView.mouse.setSensitivity(false);  //because we would not be consistent  (when dragging the mouse, we computeMouseOverList, but if there is an anim triggered by {X,Y,A}speed, and if the mouse is not moving, this list is not computed - so here we choose to disable this computation when dragging the mouse with button 3 pressed)
	activeCam=application.vsm.getActiveCamera();
    }

    public void release3(ViewPanel v,int mod,int jpx,int jpy){
	AAFViewer.vsm.animator.Xspeed=0;
	AAFViewer.vsm.animator.Yspeed=0;
	AAFViewer.vsm.animator.Aspeed=0;
	v.setDrawDrag(false);
	AAFViewer.vsm.activeView.mouse.setSensitivity(true);
    }

    public void click3(ViewPanel v,int mod,int jpx,int jpy,int clickNumber){
	Glyph g=v.lastGlyphEntered();
	if (g!=null){
	    AAFViewer.vsm.centerOnGlyph(g,v.cams[0],ConfigManager.ANIM_MOVE_LENGTH);
	}
    }

    public void mouseMoved(ViewPanel v,int jpx,int jpy){}

    public void mouseDragged(ViewPanel v,int mod,int buttonNumber,int jpx,int jpy){
	if (buttonNumber==3 || (buttonNumber==1 && (mod==NO_MODIFIER || mod==SHIFT_MOD))){
	    tfactor=(activeCam.focal+Math.abs(activeCam.altitude))/activeCam.focal;
	    if (mod==SHIFT_MOD) {
		application.vsm.animator.Xspeed=0;
		application.vsm.animator.Yspeed=0;
 		application.vsm.animator.Aspeed=(activeCam.altitude>0) ? (long)((lastJPY-jpy)*(tfactor/cfactor)) : (long)((lastJPY-jpy)/(tfactor*cfactor));  //50 is just a speed factor (too fast otherwise)
	    }
	    else {
		application.vsm.animator.Xspeed=(activeCam.altitude>0) ? (long)((jpx-lastJPX)*(tfactor/cmfactor)) : (long)((jpx-lastJPX)/(tfactor*cmfactor));
		application.vsm.animator.Yspeed=(activeCam.altitude>0) ? (long)((lastJPY-jpy)*(tfactor/cmfactor)) : (long)((lastJPY-jpy)/(tfactor*cmfactor));
		application.vsm.animator.Aspeed=0;
	    }
	}
    }

    public void enterGlyph(Glyph g){
	super.enterGlyph(g);
    }

    public void exitGlyph(Glyph g){
	super.exitGlyph(g);
    }

    public void Ktype(ViewPanel v,char c,int code,int mod){}

    public void Kpress(ViewPanel v,char c,int code,int mod){
// 	if (mod==2 && code==KeyEvent.VK_F){
// 	    AAFViewer.mainView.goFullScreen(!AAFViewer.mainView.isFullScreen(),new DisplayMode(800,600,16,DisplayMode.REFRESH_RATE_UNKNOWN));
// 	}
// 	else if (code==KeyEvent.VK_ESCAPE){
// 	    AAFViewer.mainView.goFullScreen(false,null);
// 	}
    }

    public void Krelease(ViewPanel v,char c,int code,int mod){}

    public void viewActivated(View v){}

    public void viewDeactivated(View v){}

    public void viewIconified(View v){}

    public void viewDeiconified(View v){}

    public void viewClosing(View v){application.exit();}

}
