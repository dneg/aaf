/*   FILE: ConfigManager.java
 *   DATE OF CREATION:   Thu Jan 09 14:14:35 2003
 *   AUTHOR :            Emmanuel Pietriga (emmanuel@w3.org)
 *   MODIF:              Mon Feb 03 09:44:51 2003 by Emmanuel Pietriga
 *   Copyright (c) Emmanuel Pietriga, 2002. All Rights Reserved
 *   Licensed under the GNU LGPL. For full terms see the file COPYING.
 */

package bbc.rd.aaf.aafviewer;

import java.io.File;
import java.awt.Color;
import java.awt.Font;
import org.xml.sax.*;
import org.apache.xerces.dom.DocumentImpl;
import org.apache.xerces.dom.DOMImplementationImpl;
import org.w3c.dom.*;

class ConfigManager {

    static String MAIN_TITLE="AAFViewer";

    static int mainViewW=800;
    static int mainViewH=600;
    static int mainViewX=0;
    static int mainViewY=0;

    static Font defaultFont=new Font("Dialog",0,12);

    static Color backgroundColor=Color.white;

//     static String GRAPH_ORIENTATION="LR";   //"LR" or "TB"
    static boolean SAVE_WINDOW_LAYOUT=false;
    static boolean DELETE_TEMP_FILES=true;
    static boolean ANTIALIASING=false;

    static String CMD_LINE_OPTS="";
    static String AAF2DOT_CMD_LINE_OPTS="";

    //directories
    static File m_TmpDir=new File("tmp");
    static File m_PrjDir=new File("graphs");
    static File m_DotPath=new File("C:\\Tools\\ATT\\Graphviz\\bin\\dot.exe");
    static File m_NeatoPath=new File("C:\\Tools\\ATT\\Graphviz\\bin\\neato.exe");
    static File m_GraphVizFontDir=new File("C:\\Tools\\ATT\\Graphviz");
    static File m_Aaf2DotPath=new File( "C:\\Tools\\AAF\\aaf2dot.exe" );
    static File m_LastDir=null;
    static File m_LastExportDir=null;

    /*location of the configuration file - at init time, we look for it in the user's home dir.
     If it is not there, we take the one in AAFViewer dir.*/
    static File cfgFile;
    static String PREFS_FILE_NAME="aafviewer.cfg";

    static int ANIM_MOVE_LENGTH=300;

    AAFViewer application;

    ConfigManager(AAFViewer app){
	application=app;
	cfgFile=new File(System.getProperty("user.home")+"/"+PREFS_FILE_NAME);
    }

    /*load user prefs from config file (in theory, if the file cannot be found,
      every variable should have a default value)*/
    void loadConfig(){
	if (cfgFile.exists()){
	    System.out.println("Loading Preferences from : "+cfgFile.getAbsolutePath());
	    try {
		Document d=Utils.parse(cfgFile,false);
		d.normalize();
		Element rt=d.getDocumentElement();
		Element e=(Element)(rt.getElementsByTagNameNS(AAFViewer.aafvURI,"directories")).item(0);
		try {
		    ConfigManager.m_TmpDir=new File(e.getElementsByTagNameNS(AAFViewer.aafvURI,"tmpDir").item(0).getFirstChild().getNodeValue());
		    ConfigManager.DELETE_TEMP_FILES=(new Boolean(((Element)e.getElementsByTagNameNS(AAFViewer.aafvURI,"tmpDir").item(0)).getAttribute("value"))).booleanValue();
		}
		catch (Exception ex){}
		try {ConfigManager.m_PrjDir=new File(e.getElementsByTagNameNS(AAFViewer.aafvURI,"graphDir").item(0).getFirstChild().getNodeValue());}
		catch (Exception ex){}
		try {ConfigManager.m_DotPath=new File(e.getElementsByTagNameNS(AAFViewer.aafvURI,"dot").item(0).getFirstChild().getNodeValue());}
		catch (Exception ex){}
		try {ConfigManager.m_NeatoPath=new File(e.getElementsByTagNameNS(AAFViewer.aafvURI,"neato").item(0).getFirstChild().getNodeValue());}
		catch (Exception ex){}
		try {ConfigManager.m_GraphVizFontDir=new File(e.getElementsByTagNameNS(AAFViewer.aafvURI,"graphvizFontDir").item(0).getFirstChild().getNodeValue());}
		catch (Exception ex){}
                try {ConfigManager.m_Aaf2DotPath=new File(e.getElementsByTagNameNS(AAFViewer.aafvURI,"aaf2dot").item(0).getFirstChild().getNodeValue());}
                catch (Exception ex){}
		try {
		    e=(Element)(rt.getElementsByTagNameNS(AAFViewer.aafvURI,"preferences")).item(0);
// 		    ConfigManager.GRAPH_ORIENTATION=e.getAttribute("graphOrient");
		    ConfigManager.ANTIALIASING=((new Boolean(e.getAttribute("antialiasing"))).booleanValue());
		    ConfigManager.SAVE_WINDOW_LAYOUT=(new Boolean(e.getAttribute("saveWindowLayout"))).booleanValue();
                    ConfigManager.CMD_LINE_OPTS=e.getAttribute("cmdL_options");
                    ConfigManager.AAF2DOT_CMD_LINE_OPTS=e.getAttribute("aaf2dot_cmdL_options");
		    if (ConfigManager.SAVE_WINDOW_LAYOUT){//window layout preferences
			try {
			    e=(Element)(rt.getElementsByTagNameNS(AAFViewer.aafvURI,"windows")).item(0);
			    mainViewX=(new Integer(e.getAttribute("mainX"))).intValue();
			    mainViewY=(new Integer(e.getAttribute("mainY"))).intValue();
			    mainViewW=(new Integer(e.getAttribute("mainW"))).intValue();
			    mainViewH=(new Integer(e.getAttribute("mainH"))).intValue();
			}
			catch (Exception ex2){}
		    }
		}
		catch (Exception ex){}
	    }
	    catch (Exception ex){
		System.err.println("Error while loading AAFViewer configuration file (aafviewer.cfg): ");
		ex.printStackTrace();
	    }
	}
	else {System.out.println("No Preferences File Found in : "+System.getProperty("user.home"));}
     }

    /*save user prefs to config file*/
    void saveConfig(){
 	DOMImplementation di=new DOMImplementationImpl();
	//DocumentType dtd=di.createDocumentType("isv:config",null,"isv.dtd");
	Document cfg=di.createDocument(AAFViewer.aafvURI,"aafv:config",null);
	//generate the XML document
	Element rt=cfg.getDocumentElement();
	rt.setAttribute("xmlns:aafv",AAFViewer.aafvURI);
	//save directory preferences
	Element dirs=cfg.createElementNS(AAFViewer.aafvURI,"aafv:directories");
	rt.appendChild(dirs);
	Element aDir=cfg.createElementNS(AAFViewer.aafvURI,"aafv:tmpDir");
	aDir.appendChild(cfg.createTextNode(ConfigManager.m_TmpDir.toString()));
	aDir.setAttribute("value",String.valueOf(ConfigManager.DELETE_TEMP_FILES));
	dirs.appendChild(aDir);
	aDir=cfg.createElementNS(AAFViewer.aafvURI,"aafv:graphDir");
	aDir.appendChild(cfg.createTextNode(ConfigManager.m_PrjDir.toString()));
	dirs.appendChild(aDir);
	aDir=cfg.createElementNS(AAFViewer.aafvURI,"aafv:dot");
	aDir.appendChild(cfg.createTextNode(ConfigManager.m_DotPath.toString()));
	dirs.appendChild(aDir);
	aDir=cfg.createElementNS(AAFViewer.aafvURI,"aafv:neato");
	aDir.appendChild(cfg.createTextNode(ConfigManager.m_NeatoPath.toString()));
	dirs.appendChild(aDir);
	aDir=cfg.createElementNS(AAFViewer.aafvURI,"aafv:graphvizFontDir");
	aDir.appendChild(cfg.createTextNode(ConfigManager.m_GraphVizFontDir.toString()));
        aDir=cfg.createElementNS(AAFViewer.aafvURI,"aafv:aaf2dot");
        aDir.appendChild(cfg.createTextNode(ConfigManager.m_Aaf2DotPath.toString()));
	dirs.appendChild(aDir);
	//save misc. constants
	Element consts=cfg.createElementNS(AAFViewer.aafvURI,"aafv:preferences");
	rt.appendChild(consts);
// 	consts.setAttribute("graphOrient",ConfigManager.GRAPH_ORIENTATION);
	consts.setAttribute("antialiasing",String.valueOf(ConfigManager.ANTIALIASING));
	consts.setAttribute("saveWindowLayout",String.valueOf(ConfigManager.SAVE_WINDOW_LAYOUT));
        consts.setAttribute("cmdL_options",ConfigManager.CMD_LINE_OPTS);
        consts.setAttribute("aaf2dot_cmdL_options",ConfigManager.AAF2DOT_CMD_LINE_OPTS);
	//window locations and sizes
	if (ConfigManager.SAVE_WINDOW_LAYOUT){
	    //first update the values
	    updateWindowVariables();
	    consts=cfg.createElementNS(AAFViewer.aafvURI,"aafv:windows");
	    consts.setAttribute("mainX",String.valueOf(mainViewX));
	    consts.setAttribute("mainY",String.valueOf(mainViewY));
	    consts.setAttribute("mainW",String.valueOf(mainViewW));
	    consts.setAttribute("mainH",String.valueOf(mainViewH));
	    rt.appendChild(consts);
	}
	if (cfgFile.exists()){cfgFile.delete();}
	Utils.serialize(cfg,cfgFile);
    }

    static boolean checkDot(){
	boolean res=true;
	if (!((m_TmpDir.exists()) && (m_DotPath.exists()))){res=false;}
	return res;
    }

    static boolean checkNeato(){
	boolean res=true;
	if (!((m_TmpDir.exists()) && (m_NeatoPath.exists()))){res=false;}
	return res;
    }

    static boolean checkAaf2Dot(){
        boolean res=true;
        if (!((m_TmpDir.exists()) && (m_Aaf2DotPath.exists()))){res=false;}
        return res;
    }


    static String getDirStatus(){
	StringBuffer sb=new StringBuffer();
	sb.append("Temp Directory (required): ");
	sb.append((m_TmpDir.exists()) ? m_TmpDir.toString() : "null");
	sb.append("\n");
	sb.append("Absolute Path to dot (required if using dot): ");
	sb.append((m_DotPath.exists()) ? m_DotPath.toString() : "null");
	sb.append("\n");
	sb.append("Absolute Path to neato (required if using neato): ");
	sb.append((m_NeatoPath.exists()) ? m_NeatoPath.toString() : "null");
	sb.append("\n");
	sb.append("GraphViz Font Directory (optional): ");
	sb.append((m_GraphVizFontDir.exists()) ? m_GraphVizFontDir.toString() : "null");
	sb.append("\n");
        sb.append("Absolute Path to dotexporter (required if using aaf): ");
        sb.append((m_Aaf2DotPath.exists()) ? m_Aaf2DotPath.toString() : "null");
        sb.append("\n");
	sb.append("Are you sure you want to continue?");
	return sb.toString();
    }

    /*update window position and size variables prior to saving them in the config file*/
    void updateWindowVariables(){
	mainViewX=AAFViewer.mainView.getFrame().getX();
	mainViewY=AAFViewer.mainView.getFrame().getY();
	mainViewW=AAFViewer.mainView.getFrame().getWidth();
	mainViewH=AAFViewer.mainView.getFrame().getHeight();
    }

}
