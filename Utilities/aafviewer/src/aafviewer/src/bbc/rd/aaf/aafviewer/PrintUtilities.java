/*   FILE: PrintUtilities.java
 *   DATE OF CREATION:   Fri Jan 10 09:13:53 2003
 *   AUTHOR :            Emmanuel Pietriga (emmanuel@w3.org)
 *   MODIF:              Fri Jan 10 09:21:04 2003 by Emmanuel Pietriga
 *   Copyright (c) Emmanuel Pietriga, 2002. All Rights Reserved
 *   Licensed under the GNU LGPL. For full terms see the file COPYING.
 */

package bbc.rd.aaf.aafviewer;

import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;
import java.awt.print.*;
import javax.swing.*;
import java.util.*;
import javax.swing.border.*;
import java.io.File;
import javax.imageio.*;

/**
 * This class allow the user to choose a printer and print a BufferedImage.
 */
public class PrintUtilities extends JPanel implements Printable{

    private BufferedImage bufferedImage;

    /**
     *@param bufferedImage image to be printed
     */
    public PrintUtilities( BufferedImage bufferedImage ){
	this.bufferedImage = bufferedImage;
    }


    /**
     * Launchs the printer job and call the method {@link #print(Graphics, PageFormat, int)} method
     */
    public void print(){
	PrinterJob printJob = PrinterJob.getPrinterJob();
	PageFormat pf = new PageFormat();
	pf.setOrientation(PageFormat.LANDSCAPE);
	printJob.setPrintable(this, pf);
// 	pf = printJob.validatePage(pf);
	if (printJob.printDialog()){
            try {
		printJob.print();
	    }
            catch(PrinterException pe) {
		JOptionPane.showMessageDialog(null, "Error while printing", "Error", JOptionPane.ERROR_MESSAGE);
		System.err.println("** Error printing: " + pe);
		pe.printStackTrace();
	    }
	}
    }

    public int print(Graphics g, PageFormat pf, int pageIndex){
	Graphics2D g2d=(Graphics2D)g;
	int panelWidth=bufferedImage.getWidth(); //width in pixels
	int panelHeight=bufferedImage.getHeight(); //height in pixels
	double pageHeight=pf.getImageableHeight(); //height of printer page
	double pageWidth=pf.getImageableWidth(); //width of printer page
	if (pageIndex >= 1){
	    return Printable.NO_SUCH_PAGE;
	}
	// To place the top right corner at the center of the shit page :
	setSize(new Dimension(panelWidth, panelHeight));
        g2d.translate(pf.getImageableX(),pf.getImageableY());
        g2d.translate(pf.getImageableWidth()/2, pf.getImageableHeight()/2);
        // To place at the center of the page :
	Dimension d=getSize();
        double scale=Math.min(pf.getImageableWidth()/d.width,pf.getImageableHeight()/d.height);
        if (scale < 1.0){g2d.scale(scale, scale);}
        g2d.translate(-d.width / 2.0, -d.height / 2.0);
        setOpaque(true);
	this.paint(g2d);
	return Printable.PAGE_EXISTS;
    }

    public void paint(Graphics g){
        if (getSize().width <= 0 || getSize().height <= 0){return;}
        Graphics2D g2 = (Graphics2D) g;
        if (bufferedImage != null){
            g2.drawImage(bufferedImage, 0, 0, this);
        }
    }

}
