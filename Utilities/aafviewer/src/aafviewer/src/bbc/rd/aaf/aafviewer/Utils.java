/*   FILE: Utils.java
 *   DATE OF CREATION:   Thu Jan 09 14:14:35 2003
 *   AUTHOR :            Emmanuel Pietriga (emmanuel@w3.org)
 *   MODIF:              Wed Jan 15 10:29:31 2003 by Emmanuel Pietriga
 *   Copyright (c) Emmanuel Pietriga, 2002. All Rights Reserved
 *   Licensed under the GNU LGPL. For full terms see the file COPYING.
 */

package bbc.rd.aaf.aafviewer;

import java.awt.Font;
import java.util.Enumeration;
import javax.swing.UIManager;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.FactoryConfigurationError;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;
import org.apache.xml.serialize.OutputFormat;
import org.apache.xml.serialize.XMLSerializer;
import org.apache.xml.serialize.LineSeparator;



public class Utils {

    static Font smallFont=new Font("Dialog",0,10);
    static java.awt.Color pastelBlue=new java.awt.Color(156,154,206);
    static java.awt.Color darkerPastelBlue=new java.awt.Color(125,123,165);

//     private static final String mac="com.sun.java.swing.plaf.mac.MacLookAndFeel";
//     private static final String metal="javax.swing.plaf.metal.MetalLookAndFeel";
//     private static final String motif="com.sun.java.swing.plaf.windows.WindowsLookAndFeel";
//     private static String currentLookAndFeel="com.sun.java.swing.plaf.motif.MotifLookAndFeel";

    public static void initLookAndFeel(){
// 	try {UIManager.setLookAndFeel(currentLookAndFeel);}
// 	catch(Exception ex){System.err.println("An error occured while trying to change the look and feel\n"+ex);}
	String key;
	for (Enumeration e=UIManager.getLookAndFeelDefaults().keys();e.hasMoreElements();){
	    key=(String)e.nextElement();
	    if (key.endsWith(".font") || key.endsWith("Font")){UIManager.put(key,smallFont);}
	}
	UIManager.put("ProgressBar.foreground",pastelBlue);
	UIManager.put("ProgressBar.background",java.awt.Color.lightGray);
	UIManager.put("Label.foreground",java.awt.Color.black);
    }

    /**
     * tells wheter the current JVM is version 1.4.0 and later (or not)
     */
    public static boolean javaVersionIs140OrLater(){
	String version=System.getProperty("java.vm.version");
	float numVer=(new Float(version.substring(0,3))).floatValue();
	if (numVer>=1.4f){return true;}
	else {return false;}
    }

    /**
     * Create a File object from the given directory and file names
     *
     *@param directory the file's directory
     *@param prefix the file's prefix name (not its directory)
     *@param suffix the file's suffix or extension name
     *@return a File object if a temporary file is created; null otherwise
     */
    public static File createTempFile (String directory, String prefix, String suffix){
        File f;
        try {
            File d=new File(directory);
            f=File.createTempFile(prefix,suffix,d);
        }
	catch (Exception e){e.printStackTrace();return null;}
        return f;
    }

    public static Document parse(File f,boolean validation){
	try {
	    DocumentBuilderFactory factory=DocumentBuilderFactory.newInstance();
	    factory.setValidating(validation);
	    if (!validation){factory.setAttribute("http://apache.org/xml/features/nonvalidating/load-external-dtd",new Boolean(false));}
	    factory.setNamespaceAware(true);
	    DocumentBuilder builder=factory.newDocumentBuilder();
	    Document res=builder.parse(f);
	    return res;
	}
	catch (FactoryConfigurationError e){e.printStackTrace();return null;}
	catch (ParserConfigurationException e){e.printStackTrace();return null;}
	catch (SAXException e){e.printStackTrace();return null;}
	catch (IOException e){e.printStackTrace();return null;}
    }

    public static void serialize(Document d,File f){
	OutputFormat format=new OutputFormat(d,"UTF-8",true);
 	format.setLineSeparator(LineSeparator.Web);
	try {
	    XMLSerializer serializer=new XMLSerializer(new FileWriter(f.toString()),format);
	    serializer.asDOMSerializer();
	    serializer.serialize(d);
	}
	catch (IOException e){e.printStackTrace();}
    }

}
