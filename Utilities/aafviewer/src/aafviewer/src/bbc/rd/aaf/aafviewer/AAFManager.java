package bbc.rd.aaf.aafviewer;

/**
 * <p>Title: AAFManager </p>
 * <p>Description: AAF conversion to dot file. Dot file conversion to svg.
 * SVG import.</p>
 * <p>Copyright: Copyright (c) 2003</p>
 * <p>Company: BBC Research and Development </p>
 * @author Philip de Nier
 * @version 1.0
 */

import java.io.File;

import com.xerox.VTM.svg.SVGReaderExt;
import com.xerox.VTM.svg.SVGReaderExtListener;
import com.xerox.VTM.glyphs.*;

import org.w3c.dom.Document;


public class AAFManager implements SVGReaderExtListener {

  AAFViewer application;

  File aafF;
  File dotF;
  File svgF;

  public AAFManager(AAFViewer app) {
    application = app;
  }

  void load(File f, ProgPanel pp) {
    try {
      aafF = f;
      dotF = Utils.createTempFile(ConfigManager.m_TmpDir.toString(), "zgrv",
                                  ".dot");
      pp.setPBValue(10);
      pp.setLabel("Exporting DOT from AAF file...");
      if (!callAaf2Dot())
      {
        pp.destroy();
        deleteTempFiles();
        javax.swing.JOptionPane.showMessageDialog(AAFViewer.mainView.getFrame(),
            "aaf2dot failed.");
        return;
      }
      pp.setPBValue(50);
      pp.setLabel("Converting DOT to SVG...");
      svgF = Utils.createTempFile(ConfigManager.m_TmpDir.toString(), "zgrv",
                                  ".svg");
      if (!callGraphViz()) {
        pp.destroy();
        deleteTempFiles();
        javax.swing.JOptionPane.showMessageDialog(AAFViewer.mainView.getFrame(),
            "Dot failed to export svg file from dot file.");
        return;
      }
      pp.setPBValue(80);
      pp.setLabel("Displaying SVG...");
      if (!displaySVG()) {
        pp.destroy();
        deleteTempFiles();
        javax.swing.JOptionPane.showMessageDialog(AAFViewer.mainView.getFrame(),
                                                  "Failed to display svg.");
        return;
      }
      if (ConfigManager.DELETE_TEMP_FILES) {
       deleteTempFiles();
      }
      pp.setPBValue(100);
      pp.destroy();
    }
    catch (Exception ex) {
      pp.destroy();
      deleteTempFiles();
      javax.swing.JOptionPane.showMessageDialog(AAFViewer.mainView.getFrame(),
                                                Messages.loadError + f.toString());
    }
  }

  private boolean callAaf2Dot() throws Exception {
    try {
      return generateDotFile(aafF.getAbsolutePath(), dotF.getAbsolutePath());
    }
    catch (Exception e) {
      System.err.println("Exception in aaf2dot: " +
                         e.getMessage() + "\n");
      e.printStackTrace();
      throw new Exception();
    }
  }

  private boolean callGraphViz() throws Exception {
    try {
      return generateSVGFile(dotF.getAbsolutePath(), svgF.getAbsolutePath());
    }
    catch (Exception e) {
      System.err.println("Exception generating graph: " + e.getMessage() + "\n");
      e.printStackTrace();
      throw new Exception();
    }
  }

  void deleteTempFiles() {
    if (svgF != null) {
      svgF.delete();
    }
    if (dotF != null) {
      dotF.delete();
    }
  }

  private boolean generateDotFile(String aafFilePath, String dotFilePath) throws
      Exception {
    String optionArray[] = checkAaf2DotOptions(ConfigManager.AAF2DOT_CMD_LINE_OPTS);
    String cmdArray[] = new String[optionArray.length + 5];
    cmdArray[0] = ConfigManager.m_Aaf2DotPath.toString();
    cmdArray[1] = "-aafin"; cmdArray[2] = aafFilePath;
    cmdArray[3] = "-dotout"; cmdArray[4] = dotFilePath;
    for (int i=0; i<optionArray.length; i++) {
      cmdArray[i+5] = optionArray[i];
    }
    Runtime rt = Runtime.getRuntime();
    try {
      Process proc = rt.exec(cmdArray);

      StreamGobbler errorGobbler = new
          StreamGobbler(proc.getErrorStream(), "AAF2DOT ERROR");

      StreamGobbler outputGobbler = new
          StreamGobbler(proc.getInputStream(), "AAF2DOT OUTPUT");

      errorGobbler.start();
      outputGobbler.start();

      int exitVal = proc.waitFor();

      if (exitVal != 0) {
        return false;
      }
      else {
        return true;
      }
    }
    catch (Exception e) {
      System.err.println("Error: generating OutputFile.\n");
      throw e;
    }
  }


  /**
   * Invokes the GraphViz program to create a graph image from the
   * the given DOT data file
   *@param dotFilePath the name of the DOT data file
   *@param svgFilePath the name of the output data file
   *@return true if success; false if any failure occurs
   */
  private boolean generateSVGFile(String dotFilePath, String svgFilePath) throws
      Exception {
    String cmdArray[];
    int i = 0;
    if (ConfigManager.CMD_LINE_OPTS.length() == 0 ) {
      cmdArray = new String[5];
      cmdArray[i++] = ConfigManager.m_DotPath.toString();
    }
    else {
      cmdArray = new String[6];
      cmdArray[i++] = ConfigManager.m_DotPath.toString();
      cmdArray[i++] = ConfigManager.CMD_LINE_OPTS;
    }
    cmdArray[i++] = "-Tsvg";
    cmdArray[i++] = "-o";
    cmdArray[i++] = svgFilePath;
    cmdArray[i++] = dotFilePath;

    Runtime rt = Runtime.getRuntime();
    try {
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
    catch (Exception e) {
      System.err.println("Error: generating OutputFile.\n");
      throw e;
    }
  }


    // receive messages from the SVGReaderExt when glyphs are created
    public void glyphCreated( Glyph glyph, String title ) {
	application.addGlyphTitle( glyph, title );
    }


  boolean displaySVG() throws Exception {
    try {
      SVGReaderExt svgReader = new SVGReaderExt();
      svgReader.registerListener( this );
      Document svgDoc = Utils.parse(svgF, false);
      svgReader.loadExt(svgDoc, AAFViewer.vsm, AAFViewer.mainSpace);

      return true;
    }
    catch (Exception e) {
      System.err.println("Exception displaying SVG graph: " + e.getMessage() +
                         "\n");
      e.printStackTrace();
      throw e;
    }
  }

  /* checks that the command line options do not contain -aafin or -dotout and
   splits it into an string array */
  static String[] checkAaf2DotOptions(String options) {
    int indexAAFIn = options.indexOf("-aafin");
    int indexDotOut = options.indexOf("-dotout");
    String res = options;
    if (indexAAFIn != -1 || indexDotOut != -1) {
      if (indexAAFIn != -1) {
        int i = indexAAFIn;
        res = options.substring(0, i);
        while (i < options.length() && options.charAt(i) != ' ') {
          i++;
        }
        res += options.substring(i);
      }
      if (indexDotOut != -1) {
        String res2;
        int i = indexDotOut;
        res2 = res.substring(0, i);
        while (i < res.length() && res.charAt(i) != ' ') {
          i++;
        }
        res2 += res.substring(i);
        res = res2;
      }
    }
    String resSplit[] = res.split("(\\s)+"); // options separated by 1 or more spaces
    return resSplit;
  }

  /*checks that the command line options do not contain a -Txxx */
  static String checkDotOptions(String options) {
    int i = options.indexOf("-T");
    if (i != -1) {
      String res = options.substring(0, i);
      while (i < options.length() && options.charAt(i) != ' ') {
        i++;
      }
      res += options.substring(i);
      return res;
    }
    else
      return options;
  }

}


