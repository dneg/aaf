/*   FILE: AAFViewer.java
 *   DATE OF CREATION:   Thu Jan 09 14:13:31 2003
 *   AUTHOR :            Emmanuel Pietriga (emmanuel@w3.org)
 *   MODIF:              Mon Feb 03 10:57:41 2003 by Emmanuel Pietriga
 *   Copyright (c) Emmanuel Pietriga, 2002. All Rights Reserved
 *   Licensed under the GNU LGPL. For full terms see the file COPYING.
 */

package bbc.rd.aaf.aafviewer;

import java.util.Enumeration;

import java.awt.Font;
import java.awt.Color;
import java.awt.event.*;
import javax.swing.*;
import java.util.Hashtable;
import java.util.Enumeration;
import java.util.Vector;
import java.io.File;
import javax.imageio.ImageIO;
import javax.imageio.ImageWriter;

import com.xerox.VTM.engine.*;
import com.xerox.VTM.glyphs.*;
import com.xerox.VTM.svg.SVGReader;
import net.claribole.zvtm.engine.Location;

import org.apache.xerces.dom.DOMImplementationImpl;
import org.w3c.dom.Document;

public class AAFViewer {

  static String aafvURI = "http://rd.bbc.co.uk/aafviewer";

  static VirtualSpaceManager vsm;
  static String mainSpace = "graphspace";

  static View mainView;

  static ConfigManager cfgMngr;
  static DOTManager dotMngr;

  static AAFManager aafMngr;

  AAFViewerEvtHdlr meh;

  static File cmdLineDOTFile = null;
  static String cmdLinePrg = null;

  /*remember previous camera locations so that we can get back*/
  static final int MAX_PREV_LOC = 10;
  static Vector previousLocations;

    // remember the titles associated with each glyph
    public class GlyphTitle {
	public Glyph glyph;
	public String title;

	public boolean isNode() {
	    if ( title.indexOf( "->" ) == -1 ) {
		return true;
	    }
	    return false;
	}

	public String getEdgeSource() {
	    int arrowIndex = title.indexOf( "->" );
	    if ( arrowIndex > 0 ) {
		return title.substring( 0, arrowIndex );
	    }
	    return null;
	}
	public boolean hasEdgeSource( String nodeTitle ) {
	    String edgeSource = getEdgeSource();
	    if ( edgeSource != null && edgeSource.equals( nodeTitle ) ) {
		return true;
	    }
	    return false;
	}
	public String getEdgeTarget() {
	    int arrowIndex = title.indexOf( "->" );
	    if ( arrowIndex > 0 ) {
		return title.substring( arrowIndex + 2, title.length() );
	    }
	    return null;
	}
	public boolean hasEdgeTarget( String nodeTitle ) {
	    String edgeTarget = getEdgeTarget();
	    if ( edgeTarget != null && edgeTarget.equals( nodeTitle ) ) {
		return true;
	    }
	    return false;
	}
    }
    private Vector _glyphTitles;

    public void addGlyphTitle( Glyph glyph, String title ) {
	GlyphTitle glyphTitle = new GlyphTitle();
	glyphTitle.glyph = glyph;
	glyphTitle.title = title;
	_glyphTitles.add( glyphTitle );
    }
    public Vector getGlyphTitles() {
	return _glyphTitles;
    }


  AAFViewer(boolean acc) {
    initConfig();
    //init GUI after config as we load some GUI prefs from the config file
    initGUI(acc);
    if (cmdLineDOTFile != null) {
      loadCmdLineFile();
    }
    _glyphTitles = new Vector();
  }

  void loadCmdLineFile() {
    if (cmdLinePrg != null) {
      if (cmdLinePrg.equals("-Pneato")) {
        loadFile(cmdLineDOTFile, "neato");
      }
      else if (cmdLinePrg.equals("-Pdot")) {
        loadFile(cmdLineDOTFile, "dot");
      }
      else if (cmdLinePrg.equals("-Psvg")) {
        loadSVG(cmdLineDOTFile);
      }
      else if (cmdLinePrg.equals("-Paaf")) {
        loadAAF(cmdLineDOTFile);
      }
      else {
        System.err.println("Bad option: " + cmdLinePrg);
        System.err.println("Only -Pdot, -Pneato, -Psvg -Paaf are allowed");
        System.exit(0);
      }
    }
    else {
      System.err.println("No flag specifying what program to use (-Pdot, -Pneato, -Psvg or Paaf), dot will be used");
      loadFile(cmdLineDOTFile, "dot");
    }
  }

  void initConfig() {
    dotMngr = new DOTManager(this);
    aafMngr = new AAFManager(this);
    cfgMngr = new ConfigManager(this);
    cfgMngr.loadConfig(); //have to test for existence of config file
    previousLocations = new Vector();
  }

  void initGUI(boolean acc) {
    Utils.initLookAndFeel();
    vsm = new VirtualSpaceManager();
    vsm.setMainFont(ConfigManager.defaultFont);
    vsm.setZoomLimit( -90);
    vsm.setMouseInsideGlyphColor(Color.red);
    vsm.setSelectedGlyphColor(Color.red);
    //vsm.setDebug(true);
    vsm.addVirtualSpace(mainSpace);
    vsm.addCamera(mainSpace);
    Vector vc1 = new Vector();
    vc1.add(vsm.getVirtualSpace(mainSpace).getCamera(0));
    JMenuBar jmb = initViewMenu();
    if (acc) {
      mainView = vsm.addExternalAccView(vc1, ConfigManager.MAIN_TITLE,
                                        ConfigManager.mainViewW,
                                        ConfigManager.mainViewH, true, false,
                                        jmb);
    }
    else {
      mainView = vsm.addExternalView(vc1, ConfigManager.MAIN_TITLE,
                                     ConfigManager.mainViewW,
                                     ConfigManager.mainViewH, true, false, jmb);
    }
    mainView.setLocation(ConfigManager.mainViewX, ConfigManager.mainViewY);
    mainView.setBackgroundColor(ConfigManager.backgroundColor);
    meh = new AAFViewerEvtHdlr(this);
    mainView.setEventHandler(meh);
    mainView.setVisible(true);
    setAntialiasing(ConfigManager.ANTIALIASING);
  }

  JMenuBar initViewMenu() {
    final JMenuItem openA = new JMenuItem("Open AAF file with aaf2dot...");
    openA.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_A,
                                                ActionEvent.CTRL_MASK));
    final JMenuItem openI = new JMenuItem("Open with dot...");
    openI.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_D,
                                                ActionEvent.CTRL_MASK));
    final JMenuItem open2I = new JMenuItem("Open with neato...");
    open2I.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_N,
                                                 ActionEvent.CTRL_MASK));
    final JMenuItem openS = new JMenuItem("Open SVG generated by dot/neato...");
    final JMenuItem pngI = new JMenuItem("Export to PNG (current view)...");
    final JMenuItem svgI = new JMenuItem("Export to SVG...");
    final JMenuItem printI = new JMenuItem("Print (current view)...");
    final JMenuItem exitI = new JMenuItem("Exit");
    printI.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_P,
                                                 ActionEvent.CTRL_MASK));
    final JMenuItem backI = new JMenuItem("Back");
    backI.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_B, 0));
    final JMenuItem globvI = new JMenuItem("Global View");
    globvI.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_G, 0));
    final JMenuItem fontI = new JMenuItem("Set Font...");
    final JMenuItem prefsI = new JMenuItem("Preferences...");
    final JMenuItem helpI = new JMenuItem("Commands...");
    final JMenuItem aboutI = new JMenuItem("About...");
    exitI.setAccelerator(KeyStroke.getKeyStroke(KeyEvent.VK_Q,
                                                ActionEvent.CTRL_MASK));
    ActionListener a0 = new ActionListener() {
      public void actionPerformed(ActionEvent e) {
        if (e.getSource() == openA) {
          openAAFFile();
        }
        else if (e.getSource() == openI) {
          open(0);
        }
        else if (e.getSource() == open2I) {
          open(1);
        }
        else if (e.getSource() == openS) {
          openSVGFile();
        }
        else if (e.getSource() == globvI) {
          getGlobalView();
        }
        else if (e.getSource() == backI) {
          moveBack();
        }
        else if (e.getSource() == fontI) {
          assignFontToGraph();
        }
        else if (e.getSource() == pngI) {
          savePNG();
        }
        else if (e.getSource() == svgI) {
          saveSVG();
        }
        else if (e.getSource() == printI) {
          print();
        }
        else if (e.getSource() == prefsI) {
          showPreferences();
        }
        else if (e.getSource() == exitI) {
          exit();
        }
        else if (e.getSource() == helpI) {
          help();
        }
        else if (e.getSource() == aboutI) {
          about();
        }
      }
    };
    JMenuBar jmb = new JMenuBar();
    JMenu jm1 = new JMenu("File");
    JMenu jm2 = new JMenu("View");
    JMenu jm3 = new JMenu("Help");
    jmb.add(jm1);
    jmb.add(jm2);
    jmb.add(jm3);
    jm1.add(openA);
    jm1.add(openI);
    jm1.add(open2I);
    jm1.add(openS);
    jm1.addSeparator();
    jm1.add(pngI);
    jm1.add(svgI);
    jm1.addSeparator();
    jm1.add(printI);
    jm1.addSeparator();
    jm1.add(exitI);
    jm2.add(backI);
    jm2.add(globvI);
    jm2.addSeparator();
    jm2.add(fontI);
    jm2.addSeparator();
    jm2.add(prefsI);
    jm3.add(helpI);
    jm3.add(aboutI);
    openI.addActionListener(a0);
    open2I.addActionListener(a0);
    openS.addActionListener(a0);
    openA.addActionListener(a0);
    pngI.addActionListener(a0);
    svgI.addActionListener(a0);
    printI.addActionListener(a0);
    exitI.addActionListener(a0);
    globvI.addActionListener(a0);
    backI.addActionListener(a0);
    fontI.addActionListener(a0);
    prefsI.addActionListener(a0);
    helpI.addActionListener(a0);
    aboutI.addActionListener(a0);
    return jmb;
  }

  void reset() {
    vsm.destroyGlyphsInSpace(mainSpace);
    previousLocations.removeAllElements();
  }

  void open(int dotOrNeato) { //0=dot 1=neato
    if (dotOrNeato == 0) {
      if (ConfigManager.checkDot()) {
        openDotFile();
      }
      else {
        Object[] options = {
            "Yes", "No"};
        int option = JOptionPane.showOptionDialog(null,
                                                  ConfigManager.getDirStatus(),
                                                  "Warning",
                                                  JOptionPane.DEFAULT_OPTION,
                                                  JOptionPane.WARNING_MESSAGE, null,
                                                  options, options[0]);
        if (option == JOptionPane.OK_OPTION) {
          openDotFile();
        }
      }
    }
    else {
      if (ConfigManager.checkNeato()) {
        openNeatoFile();
      }
      else {
        Object[] options = {
            "Yes", "No"};
        int option = JOptionPane.showOptionDialog(null,
                                                  ConfigManager.getDirStatus(),
                                                  "Warning",
                                                  JOptionPane.DEFAULT_OPTION,
                                                  JOptionPane.WARNING_MESSAGE, null,
                                                  options, options[0]);
        if (option == JOptionPane.OK_OPTION) {
          openNeatoFile();
        }
      }
    }
  }

  void openDotFile() {
    final JFileChooser fc = new JFileChooser(ConfigManager.m_LastDir != null ?
                                             ConfigManager.m_LastDir :
                                             ConfigManager.m_PrjDir);
    fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
    fc.setDialogTitle("Find DOT File (dot)");
    int returnVal = fc.showOpenDialog(mainView.getFrame());
    if (returnVal == JFileChooser.APPROVE_OPTION) {
      final SwingWorker worker = new SwingWorker() {
        public Object construct() {
          reset();
          loadFile(fc.getSelectedFile(), "dot");
          return null;
        }
      };
      worker.start();
    }
  }

  void openNeatoFile() {
    final JFileChooser fc = new JFileChooser(ConfigManager.m_LastDir != null ?
                                             ConfigManager.m_LastDir :
                                             ConfigManager.m_PrjDir);
    fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
    fc.setDialogTitle("Find DOT File (neato)");
    int returnVal = fc.showOpenDialog(mainView.getFrame());
    if (returnVal == JFileChooser.APPROVE_OPTION) {
      final SwingWorker worker = new SwingWorker() {
        public Object construct() {
          reset();
          loadFile(fc.getSelectedFile(), "neato");
          return null;
        }
      };
      worker.start();
    }
  }

  void openSVGFile() {
    final JFileChooser fc = new JFileChooser(ConfigManager.m_LastDir != null ?
                                             ConfigManager.m_LastDir :
                                             ConfigManager.m_PrjDir);
    fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
    fc.setDialogTitle("Find SVG File");
    int returnVal = fc.showOpenDialog(mainView.getFrame());
    if (returnVal == JFileChooser.APPROVE_OPTION) {
      final SwingWorker worker = new SwingWorker() {
        public Object construct() {
          reset();
          loadSVG(fc.getSelectedFile());
          return null;
        }
      };
      worker.start();
    }
  }

  void openAAFFile() {
    if (!ConfigManager.checkAaf2Dot()) {
      JOptionPane.showMessageDialog(mainView.getFrame(),
                                    "The aaf2dot command does not exist as " +
                                    ConfigManager.m_Aaf2DotPath + "\n");
      return;
    }
    final JFileChooser fc = new JFileChooser(ConfigManager.m_LastDir != null ?
                                             ConfigManager.m_LastDir :
                                             ConfigManager.m_PrjDir);
    fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
    fc.setDialogTitle("Find AAF File");
    int returnVal = fc.showOpenDialog(mainView.getFrame());
    if (returnVal == JFileChooser.APPROVE_OPTION) {
      final SwingWorker worker = new SwingWorker() {
        public Object construct() {
          reset();
          loadAAF(fc.getSelectedFile());
          return null;
        }
      };
      worker.start();
    }
  }

  void loadFile(File f, String prg) { //f=DOT file to load, prg=program to use ("dot" or "neato")
    if (f.exists()) {
      ConfigManager.m_LastDir = f.getParentFile();
      dotMngr.load(f, prg);
      //in case a font was defined in the SVG file, make it the font used here (to show in Prefs)
      ConfigManager.defaultFont = vsm.getMainFont();
      mainView.setTitle(ConfigManager.MAIN_TITLE + " - " + f.getAbsolutePath());
      getGlobalView();
      if (previousLocations.size() == 1) {
        previousLocations.removeElementAt(0);
      } //do not remember camera's initial location (before global view)
      //debug - coordinate system
// 	    VSegment d=new VSegment(0,0,0,10000,0,Color.yellow);
// 	    vsm.addGlyph(d,mainSpace);
// 	    d=new VSegment(0,0,0,0,10000,Color.yellow);
// 	    vsm.addGlyph(d,mainSpace);
    }
  }

  void loadSVG(File f) {
    ProgPanel pp = new ProgPanel("Parsing SVG...", "Loading SVG File");
    try {
      pp.setPBValue(30);
      Document svgDoc = Utils.parse(f, false);
      pp.setLabel("Displaying...");
      pp.setPBValue(80);
      SVGReader.load(svgDoc, AAFViewer.vsm, AAFViewer.mainSpace);
      ConfigManager.defaultFont = vsm.getMainFont();
      mainView.setTitle(ConfigManager.MAIN_TITLE + " - " + f.getAbsolutePath());
      getGlobalView();
      if (previousLocations.size() == 1) {
        previousLocations.removeElementAt(0);
      } //do not remember camera's initial location (before global view)
      pp.destroy();
    }
    catch (Exception ex) {
      pp.destroy();
      JOptionPane.showMessageDialog(mainView.getFrame(),
                                    Messages.loadError + f.toString());
    }
  }

  void loadAAF(File f) {
    ProgPanel pp = new ProgPanel("", "Loading AAF File");
    try {
      aafMngr.load(f, pp);
      ConfigManager.defaultFont = vsm.getMainFont();
      mainView.setTitle(ConfigManager.MAIN_TITLE + " - " + f.getAbsolutePath());
      getGlobalView();
      if (previousLocations.size() == 1) {
        previousLocations.removeElementAt(0);
      } //do not remember camera's initial location (before global view)
      pp.destroy();
    }
    catch (Exception ex) {
      pp.destroy();
      JOptionPane.showMessageDialog(mainView.getFrame(),
                                    Messages.loadError + f.toString());
    }
  }

  void getGlobalView() {
    Location l = vsm.getGlobalView(vsm.getActiveCamera(),
                                   ConfigManager.ANIM_MOVE_LENGTH);
    rememberLocation(vsm.getActiveCamera().getLocation());
  }

  void rememberLocation(Location l) {
    if (previousLocations.size() >= MAX_PREV_LOC) { // as a result of release/click being undifferentiated)
      previousLocations.removeElementAt(0);
    }
    if (previousLocations.size() > 0) {
      if (!Location.equals( (Location) previousLocations.lastElement(), l)) {
        previousLocations.add(l);
      }
    }
    else {
      previousLocations.add(l);
    }
  }

  void moveBack() {
    if (previousLocations.size() > 0) {
      Location newlc = (Location) previousLocations.lastElement();
      Location currentlc = vsm.getActiveCamera().getLocation();
      Vector animParams = Location.getDifference(currentlc, newlc);
      vsm.animator.createCameraAnimation(ConfigManager.ANIM_MOVE_LENGTH,
                                         AnimManager.CA_ALT_TRANS_SIG,
                                         animParams,
                                         vsm.getActiveCamera().getID());
      previousLocations.removeElementAt(previousLocations.size() - 1);
    }
  }

  void savePNG() {
    final JFileChooser fc = new JFileChooser(ConfigManager.m_LastExportDir != null ?
                                             ConfigManager.m_LastExportDir :
                                             ConfigManager.m_PrjDir);
    fc.setDialogTitle("Export PNG");
    int returnVal = fc.showSaveDialog(mainView.getFrame());
    if (returnVal == JFileChooser.APPROVE_OPTION) {
      final SwingWorker worker = new SwingWorker() {
        public Object construct() {
          exportPNG(fc.getSelectedFile());
          return null;
        }
      };
      worker.start();
    }
  }

  /*export as PNG (bitmap image) locally (only the current view displayed by VTM, not the entire virtual space)*/
  public void exportPNG(File f) { //should only be called if JVM is 1.4.0-beta or later (uses package javax.imageio)
    //comment out this method if trying to compile using a JDK 1.3.x
    boolean proceed = true;
    if (!Utils.javaVersionIs140OrLater()) {
      Object[] options = {
          "Yes", "No"};
      int option = JOptionPane.showOptionDialog(null,
                                                Messages.pngOnlyIn140FirstPart +
                                                System.getProperty(
          "java.vm.version") + Messages.pngOnlyIn140SecondPart, "Warning",
                                                JOptionPane.DEFAULT_OPTION,
                                                JOptionPane.WARNING_MESSAGE, null,
                                                options, options[0]);
      if (option != JOptionPane.OK_OPTION) {
        proceed = false;
      }
    }
    if (proceed) {
      mainView.setCursorIcon(java.awt.Cursor.WAIT_CURSOR);
      ConfigManager.m_LastExportDir = f.getParentFile();
      mainView.setStatusBarText("Exporting to PNG " + f.toString() +
                                " ... (This operation can take some time)");
      ImageWriter writer = (ImageWriter) ImageIO.getImageWritersByFormatName(
          "png").next();
      try {
        writer.setOutput(ImageIO.createImageOutputStream(f));
        java.awt.image.BufferedImage bi = mainView.getImage();
        if (bi != null) {
          writer.write(bi);
          writer.dispose();
          mainView.setStatusBarText("Exporting to PNG " + f.toString() +
                                    " ...done");
        }
        else {
          JOptionPane.showMessageDialog(mainView.getFrame(),
              "An error occured when retrieving the image.\n Please try again.");
        }
      }
      catch (java.io.IOException ex) {
        JOptionPane.showMessageDialog(mainView.getFrame(),
                                      "Error while exporting to PNG:\n" + ex);
      }
      mainView.setCursorIcon(java.awt.Cursor.CUSTOM_CURSOR);
    }
  }

  void saveSVG() {
    final JFileChooser fc = new JFileChooser(ConfigManager.m_LastExportDir != null ?
                                             ConfigManager.m_LastExportDir :
                                             ConfigManager.m_PrjDir);
    fc.setDialogTitle("Export SVG");
    int returnVal = fc.showSaveDialog(mainView.getFrame());
    if (returnVal == JFileChooser.APPROVE_OPTION) {
      final SwingWorker worker = new SwingWorker() {
        public Object construct() {
          exportSVG(fc.getSelectedFile());
          return null;
        }
      };
      worker.start();
    }
  }

  /*export the entire RDF graph as SVG locally*/
  public void exportSVG(File f) {
    if (f != null) {
      mainView.setCursorIcon(java.awt.Cursor.WAIT_CURSOR);
      ConfigManager.m_LastExportDir = f.getParentFile();
      mainView.setStatusBarText("Exporting to SVG " + f.toString() + " ...");
      if (f.exists()) {
        f.delete();
      }
      com.xerox.VTM.svg.SVGWriter svgw = new com.xerox.VTM.svg.SVGWriter();
      Document d = svgw.exportVirtualSpace(vsm.getVirtualSpace(mainSpace),
                                           new DOMImplementationImpl(), f);
      Utils.serialize(d, f);
      mainView.setStatusBarText("Exporting to SVG " + f.toString() + " ...done");
      mainView.setCursorIcon(java.awt.Cursor.CUSTOM_CURSOR);
    }
  }

  /*opens a print dialog box*/
  void print() {
    java.awt.image.BufferedImage bi = mainView.getImage();
    if (bi != null) {
      PrintUtilities pu = new PrintUtilities(bi);
      pu.print();
    }
  }

  void assignFontToGraph() {
    Font f = net.claribole.zvtm.fonts.FontDialog.getFontDialog( (JFrame)
        mainView.getFrame(), ConfigManager.defaultFont);
    if (f != null) {
      ConfigManager.defaultFont = f;
      vsm.setMainFont(ConfigManager.defaultFont);
    }
  }

  void showPreferences() {
    PrefWindow dp = new PrefWindow(this);
    dp.setLocation(mainView.getFrame().getLocation());
    dp.setVisible(true);
  }

  /*antialias ON/OFF for views*/
  void setAntialiasing(boolean b) {
    ConfigManager.ANTIALIASING = b;
    mainView.setAntialiasing(ConfigManager.ANTIALIASING);
  }

  void saveConfiguration() {
    cfgMngr.saveConfig();
  }

  void help() {
    TextViewer tv = new TextViewer(new StringBuffer(Messages.commands),
                                   "Commands", 0);
    tv.setLocation(mainView.getFrame().getLocation());
    tv.setVisible(true);
  }

  void about() {
    javax.swing.JOptionPane.showMessageDialog(mainView.getFrame(),
                                              Messages.about);
  }

  void exit() {
    System.exit(0);
  }

  public static void main(String[] args) {
    boolean acceleratedView = false;
 /*
   for (int i = 0; i < args.length; i++) {
      if (args[i].startsWith("-")) {
        if (args[i].equals("--help")) {
          System.out.println(
              "\n\njava bbc.rd.aaf.aafviewer.AAFViewer [options] [file]");
          System.out.println("[options] -acc ZVTM will run in Accelerated mode");
          System.out.println("          -Pxxx where xxx=dot, neato or svg to specify what program to use to compute the [file]'s layout");
          System.out.println(
              "[file]    can be a relative or full path ; use the native OS syntax\n\n");
          System.exit(0);
        }
        else if (args[i].equals("-acc")) {
          System.out.println("ZVTM: Accelerated mode ON");
          acceleratedView = true;
        }
        else if (args[i].startsWith("-P")) {
          cmdLinePrg = args[i];
        }
      }
      else { //the only other stuff allowed as a cmd line param is a dot file
        File f = new File(args[i]);
        if (f.exists()) {
          cmdLineDOTFile = f;
        }
      }
    }
    System.out.println("--help for command line options");
 */
    AAFViewer appli = new AAFViewer(acceleratedView);
  }

}
