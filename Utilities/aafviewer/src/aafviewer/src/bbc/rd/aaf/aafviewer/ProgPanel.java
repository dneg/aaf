/*   $Id$ $Name$
 *   FILE: ProgPanel.java
 *   DATE OF CREATION:   Fri Jan 10 08:41:44 2003
 *   AUTHOR :            Emmanuel Pietriga (emmanuel@w3.org)
 *   MODIF:              Fri Jan 10 08:42:21 2003 by Emmanuel Pietriga
 *   Copyright (c) Emmanuel Pietriga, 2002. All Rights Reserved
 *   Licensed under the GNU LGPL. For full terms see the file COPYING.
 */


package bbc.rd.aaf.aafviewer;

import javax.swing.*;
import java.awt.*;

//implements a JFrame with a progress bar with methods to change the bar value and the text displayed just above it

public class ProgPanel extends JFrame {

    JLabel l1;
    JProgressBar jpb;

    ProgPanel(String label,String title){
	Container cpane=this.getContentPane();
	GridBagLayout gridBag=new GridBagLayout();
	GridBagConstraints constraints=new GridBagConstraints();
	constraints.fill=GridBagConstraints.HORIZONTAL;
	constraints.anchor=GridBagConstraints.WEST;
	cpane.setLayout(gridBag);
	l1=new JLabel(label);
	buildConstraints(constraints,0,0,1,1,100,50);
	gridBag.setConstraints(l1,constraints);
	cpane.add(l1);
	jpb=new JProgressBar();
	jpb.setMinimum(0);
	jpb.setMaximum(100);
	jpb.setStringPainted(false);
	buildConstraints(constraints,0,1,1,1,100,50);
	gridBag.setConstraints(jpb,constraints);
	cpane.add(jpb);
	this.setTitle(title);
	this.pack();
	Dimension screenSize=java.awt.Toolkit.getDefaultToolkit().getScreenSize();
	this.setLocation((screenSize.width-300)/2,(screenSize.height-100)/2);
	this.setSize(300,100);
	this.setVisible(true);
    }

    void setPBValue(int i){
	jpb.setValue(i);
    }

    void setLabel(String s){
	l1.setText(s);
    }

    void destroy(){
	this.setVisible(false);
	this.dispose();
    }

    void buildConstraints(GridBagConstraints gbc, int gx,int gy,int gw,int gh,int wx,int wy){
	gbc.gridx=gx;
	gbc.gridy=gy;
	gbc.gridwidth=gw;
	gbc.gridheight=gh;
	gbc.weightx=wx;
	gbc.weighty=wy;
    }

}
