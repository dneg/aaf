/*   $Id$ $Name$
 *   FILE: DOTManager.java
 *   DATE OF CREATION:   Thu Jan 09 14:14:35 2003
 *   AUTHOR :            Emmanuel Pietriga (emmanuel@w3.org)
 *   MODIF:              Fri Jan 17 10:41:41 2003 by Emmanuel Pietriga
 *   Copyright (c) Emmanuel Pietriga, 2002. All Rights Reserved
 *   Licensed under the GNU LGPL. For full terms see the file COPYING.
 */

package bbc.rd.aaf.aafviewer;

import java.io.File;

import com.xerox.VTM.svg.SVGReader;
import org.w3c.dom.Document;


class DOTManager {

    AAFViewer application;

    File dotF;
    File svgF;

    DOTManager(AAFViewer app){
	application=app;
    }

    void load(File f,String prg){//"dot" or "neato"
	ProgPanel pp=new ProgPanel("Resetting...","Loading DOT File");
	try {
	    svgF=Utils.createTempFile(ConfigManager.m_TmpDir.toString(),"zgrv",".svg");
	    dotF=f;
	    callGraphViz(pp,prg);
	    pp.setLabel("Deleting Temp File...");
	    pp.setPBValue(100);
	    pp.destroy();
	}
	catch (Exception ex){
	    pp.destroy();
	    javax.swing.JOptionPane.showMessageDialog(AAFViewer.mainView.getFrame(),Messages.loadError+f.toString());
	}
    }

    private void callGraphViz(ProgPanel pp,String prg) throws Exception {
        try {
	    pp.setLabel("Preparing SVG Temp File");
	    pp.setPBValue(10);
            if (!generateSVGFile(dotF.getAbsolutePath(),svgF.getAbsolutePath(),pp,prg)) {
		deleteTempFiles();
                return;
            }
	    else {
		displaySVG(pp);
		if (ConfigManager.DELETE_TEMP_FILES){deleteTempFiles();}
	    }
        }
	catch (Exception e){
	    System.err.println("Exception generating graph: " + e.getMessage()+"\n");
	    e.printStackTrace();
	    throw new Exception();
	}
    }

    void deleteTempFiles(){
	if (svgF!=null){svgF.delete();}
    }

    /**
     * Invokes the GraphViz program to create a graph image from the
     * the given DOT data file
     *@param dotFilePath the name of the DOT data file
     *@param svgFilePath the name of the output data file
     *@param prg program to use (dot or neato)

     *@return true if success; false if any failure occurs
     */
    private boolean generateSVGFile(String dotFilePath,String svgFilePath,ProgPanel pp,String prg){
        //String environment[]={DOTFONTPATH+"="+Editor.m_GraphVizFontDir};
        String cmdArray[]={(prg.equals("dot")) ? ConfigManager.m_DotPath.toString() : ConfigManager.m_NeatoPath.toString(),"-Tsvg",checkOptions(ConfigManager.CMD_LINE_OPTS),"-o",svgFilePath,dotFilePath};
        Runtime rt=Runtime.getRuntime();
	pp.setLabel("Computing Graph Layout (GraphViz)...");
	pp.setPBValue(40);
        try {
            //Process p = rt.exec(cmdArray, environment);
//            Process p = rt.exec(cmdArray);
//            p.waitFor();
            Process proc = rt.exec(cmdArray);

           StreamGobbler errorGobbler = new
                StreamGobbler(proc.getErrorStream(), "DOT ERROR");

            StreamGobbler outputGobbler = new
                StreamGobbler(proc.getInputStream(), "DOT OUTPUT");

            errorGobbler.start();
            outputGobbler.start();

            int exitVal = proc.waitFor();

            return true;
        }
	catch (Exception e) {System.err.println("Error: generating OutputFile.\n");return false;}
    }

    void displaySVG(ProgPanel pp){
	pp.setLabel("Parsing SVG...");
	pp.setPBValue(60);
	Document svgDoc=Utils.parse(svgF,false);
	pp.setLabel("Displaying...");
	pp.setPBValue(80);
	SVGReader.load(svgDoc,AAFViewer.vsm,AAFViewer.mainSpace);
    }

    /*checks that the command line options do not contain a -Txxx */
    static String checkOptions(String options){
	int i=options.indexOf("-T");
	if (i!=-1){
	    String res=options.substring(0,i);
	    while (i<options.length() && options.charAt(i)!=' '){i++;}
	    res+=options.substring(i);
	    return res;
	}
	else return options;
    }

}
