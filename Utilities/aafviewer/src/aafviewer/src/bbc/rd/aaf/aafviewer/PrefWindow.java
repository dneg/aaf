/*   FILE: PrefWindow.java
 *   DATE OF CREATION:   Thu Jan 09 15:47:07 2003
 *   AUTHOR :            Emmanuel Pietriga (emmanuel@w3.org)
 *   MODIF:              Mon Feb 03 11:04:08 2003 by Emmanuel Pietriga
 *   Copyright (c) Emmanuel Pietriga, 2002. All Rights Reserved
 *   Licensed under the GNU LGPL. For full terms see the file COPYING.
 */

package bbc.rd.aaf.aafviewer;

import javax.swing.*;
import java.awt.event.*;
import javax.swing.event.*;
import java.awt.*;
import java.io.File;

class PrefWindow extends JFrame implements ActionListener {

    AAFViewer application;

    JTabbedPane tabbedPane;

    JButton okPrefs,savePrefs;

    //Misc prefs
    JCheckBox saveWindowLayoutCb;
    JCheckBox antialiascb; //set antialias rendering
    JTextField cmdLOptsTf;
    JTextField dotECmdLOptsTf;

    //directory panel
    JButton brw1,brw2,brw3,brw4,brw5,brw6;
    JTextField tf1,tf2,tf3,tf4,tf5,tf6;
    JCheckBox cb1;

    PrefWindow(AAFViewer app){
	this.application=app;

	tabbedPane = new JTabbedPane();

	//misc panel
	JPanel miscPane=new JPanel();
	GridBagLayout gridBag0=new GridBagLayout();
	GridBagConstraints constraints0=new GridBagConstraints();
	constraints0.fill=GridBagConstraints.HORIZONTAL;
	constraints0.anchor=GridBagConstraints.WEST;
	miscPane.setLayout(gridBag0);

// 	JLabel lb2=new JLabel("Graph Orientation");
// 	buildConstraints(constraints0,0,0,1,1,34,10);
// 	gridBag0.setConstraints(lb2,constraints0);
// 	miscPane.add(lb2);
// 	ButtonGroup bg1=new ButtonGroup();
// 	b1a=new JRadioButton("Horizontal");
// 	b2a=new JRadioButton("Vertical");
// 	bg1.add(b1a);
// 	bg1.add(b2a);
// 	if (ConfigManager.GRAPH_ORIENTATION.equals("LR")){b1a.setSelected(true);} else {b2a.setSelected(true);}
// 	buildConstraints(constraints0,1,0,1,1,33,0);
// 	gridBag0.setConstraints(b1a,constraints0);
// 	miscPane.add(b1a);
// 	buildConstraints(constraints0,2,0,1,1,33,0);
// 	gridBag0.setConstraints(b2a,constraints0);
// 	miscPane.add(b2a);
	//save window layout checkbox
	saveWindowLayoutCb=new JCheckBox("Save/Restore Window Layout at Startup",ConfigManager.SAVE_WINDOW_LAYOUT);
	buildConstraints(constraints0,0,0,1,1,100,10);
	gridBag0.setConstraints(saveWindowLayoutCb,constraints0);
	miscPane.add(saveWindowLayoutCb);
	//antialiasing
	antialiascb=new JCheckBox("Antialiasing",ConfigManager.ANTIALIASING);
	antialiascb.addActionListener(this);
	buildConstraints(constraints0,0,1,1,1,100,10);
	gridBag0.setConstraints(antialiascb,constraints0);
	miscPane.add(antialiascb);
        //command line options
        JLabel cmdLOptsLb=new JLabel("dot/neato command line options (-T will be ignored)");
        buildConstraints(constraints0,0,2,1,1,100,10);
        gridBag0.setConstraints(cmdLOptsLb,constraints0);
        miscPane.add(cmdLOptsLb);
        cmdLOptsTf=new JTextField(ConfigManager.CMD_LINE_OPTS);
        buildConstraints(constraints0,0,3,1,1,100,10);
        gridBag0.setConstraints(cmdLOptsTf,constraints0);
        miscPane.add(cmdLOptsTf);
        //dot exporter command line options
        JLabel dotECmdLOptsLb=new JLabel("aaf2dot command line options (-aafin and -dotout will be ignored)");
        buildConstraints(constraints0,0,4,1,1,100,10);
        gridBag0.setConstraints(dotECmdLOptsLb,constraints0);
        miscPane.add(dotECmdLOptsLb);
        dotECmdLOptsTf=new JTextField(ConfigManager.AAF2DOT_CMD_LINE_OPTS);
        buildConstraints(constraints0,0,5,1,1,100,10);
        gridBag0.setConstraints(dotECmdLOptsTf,constraints0);
        miscPane.add(dotECmdLOptsTf);
	//blank panel to fill remaining part of the tab
	JPanel p1=new JPanel();
	buildConstraints(constraints0,0,6,1,1,100,70);
	gridBag0.setConstraints(p1,constraints0);
	miscPane.add(p1);
	//add tab to panel
	tabbedPane.addTab("Misc.",miscPane);

	//directories panel
	JPanel dirPane=new JPanel();
	GridBagLayout gridBag=new GridBagLayout();
	GridBagConstraints constraints=new GridBagConstraints();
	constraints.fill=GridBagConstraints.HORIZONTAL;
	constraints.anchor=GridBagConstraints.WEST;
	dirPane.setLayout(gridBag);
	JLabel l1=new JLabel("Temporary directory");
	buildConstraints(constraints,0,0,1,1,60,10);
	gridBag.setConstraints(l1,constraints);
	dirPane.add(l1);
	cb1=new JCheckBox("Delete temp files on exit");
	buildConstraints(constraints,1,0,1,1,30,0);
	gridBag.setConstraints(cb1,constraints);
	if (ConfigManager.DELETE_TEMP_FILES){cb1.setSelected(true);} else {cb1.setSelected(false);}
	cb1.addActionListener(this);
	dirPane.add(cb1);
	brw1=new JButton("Browse...");
	buildConstraints(constraints,2,0,1,1,10,0);
	gridBag.setConstraints(brw1,constraints);
	brw1.addActionListener(this);
	dirPane.add(brw1);
	tf1=new JTextField(ConfigManager.m_TmpDir.toString());tf1.setEnabled(false);
	buildConstraints(constraints,0,1,3,1,100,10);
	gridBag.setConstraints(tf1,constraints);
	dirPane.add(tf1);
	JLabel l2=new JLabel("data files directory");
	buildConstraints(constraints,0,2,2,1,90,10);
	gridBag.setConstraints(l2,constraints);
	dirPane.add(l2);
	brw2=new JButton("Browse...");
	buildConstraints(constraints,2,2,1,1,10,0);
	gridBag.setConstraints(brw2,constraints);
	brw2.addActionListener(this);
	dirPane.add(brw2);
	tf2=new JTextField(ConfigManager.m_PrjDir.toString());tf2.setEnabled(false);
	buildConstraints(constraints,0,3,3,1,100,10);
	gridBag.setConstraints(tf2,constraints);
	dirPane.add(tf2);
	JLabel l4=new JLabel("GraphViz/dot executable");
	buildConstraints(constraints,0,4,2,1,90,10);
	gridBag.setConstraints(l4,constraints);
	dirPane.add(l4);
	brw4=new JButton("Browse...");
	buildConstraints(constraints,2,4,1,1,10,0);
	gridBag.setConstraints(brw4,constraints);
	brw4.addActionListener(this);
	dirPane.add(brw4);
	tf4=new JTextField(ConfigManager.m_DotPath.toString());tf4.setEnabled(false);
	buildConstraints(constraints,0,5,3,1,100,10);
	gridBag.setConstraints(tf4,constraints);
	dirPane.add(tf4);
	JLabel l3=new JLabel("GraphViz/neato executable");
	buildConstraints(constraints,0,6,2,1,90,10);
	gridBag.setConstraints(l3,constraints);
	dirPane.add(l3);
	brw3=new JButton("Browse...");
	buildConstraints(constraints,2,6,1,1,10,0);
	gridBag.setConstraints(brw3,constraints);
	brw3.addActionListener(this);
	dirPane.add(brw3);
	tf3=new JTextField(ConfigManager.m_NeatoPath.toString());tf3.setEnabled(false);
	buildConstraints(constraints,0,7,3,1,100,10);
	gridBag.setConstraints(tf3,constraints);
	dirPane.add(tf3);
	JLabel l5=new JLabel("GraphViz font directory (optional)");
	buildConstraints(constraints,0,8,2,1,90,10);
	gridBag.setConstraints(l5,constraints);
	dirPane.add(l5);
	brw5=new JButton("Browse...");
	buildConstraints(constraints,2,8,1,1,10,0);
	gridBag.setConstraints(brw5,constraints);
	brw5.addActionListener(this);
	dirPane.add(brw5);
	tf5=new JTextField(ConfigManager.m_GraphVizFontDir.toString());tf5.setEnabled(false);
	buildConstraints(constraints,0,9,3,1,100,10);
	gridBag.setConstraints(tf5,constraints);
	dirPane.add(tf5);
        JLabel l6=new JLabel("aaf2dot executable");
        buildConstraints(constraints,0,10,2,1,90,10);
        gridBag.setConstraints(l6,constraints);
        dirPane.add(l6);
        brw6=new JButton("Browse...");
        buildConstraints(constraints,2,10,1,1,10,0);
        gridBag.setConstraints(brw6,constraints);
        brw6.addActionListener(this);
        dirPane.add(brw6);
        tf6=new JTextField(ConfigManager.m_Aaf2DotPath.toString());tf6.setEnabled(false);
        buildConstraints(constraints,0,11,3,1,100,10);
        gridBag.setConstraints(tf6,constraints);
        dirPane.add(tf6);
	tabbedPane.addTab("Directories",dirPane);



	//main panel (tabbed panes + OK/Save buttons)
	Container cpane=this.getContentPane();
	GridBagLayout gridBag3=new GridBagLayout();
	GridBagConstraints constraints3=new GridBagConstraints();
	constraints3.fill=GridBagConstraints.BOTH;
	constraints3.anchor=GridBagConstraints.WEST;
	cpane.setLayout(gridBag3);
	buildConstraints(constraints3,0,0,3,1,100,90);
	gridBag3.setConstraints(tabbedPane,constraints3);
	cpane.add(tabbedPane);
	JPanel tmp=new JPanel();
	buildConstraints(constraints3,0,1,1,1,70,10);
	gridBag3.setConstraints(tmp,constraints3);
	cpane.add(tmp);
	constraints3.fill=GridBagConstraints.HORIZONTAL;
	constraints3.anchor=GridBagConstraints.CENTER;
	okPrefs=new JButton("Apply & Close");
	//okPrefs.setPreferredSize(new Dimension(60,25));
	buildConstraints(constraints3,1,1,1,1,15,10);
	gridBag3.setConstraints(okPrefs,constraints3);
	okPrefs.addActionListener(this);
	cpane.add(okPrefs);
	constraints3.fill=GridBagConstraints.HORIZONTAL;
	constraints3.anchor=GridBagConstraints.CENTER;
	savePrefs=new JButton("Save");
	//savePrefs.setPreferredSize(new Dimension(60,35));
	buildConstraints(constraints3,2,1,1,1,15,10);
	gridBag3.setConstraints(savePrefs,constraints3);
	savePrefs.addActionListener(this);
	cpane.add(savePrefs);

	tabbedPane.setSelectedIndex(0);
	//window
	WindowListener w0=new WindowAdapter(){
		public void windowClosing(WindowEvent e){}
	    };
	this.addWindowListener(w0);
	this.setTitle("Preferences");
	this.pack();
	this.setSize(400,300);
    }

    public void actionPerformed(ActionEvent e){
	JFileChooser fc;
	int returnVal;
	Object o=e.getSource();
	if (o==brw1){//tmp directory browse button
	    fc=new JFileChooser(ConfigManager.m_TmpDir);
 	    fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY); //does not work well with JVM 1.3.x (works fine in 1.4)
	    returnVal= fc.showOpenDialog(this);
	    if (returnVal == JFileChooser.APPROVE_OPTION){
		ConfigManager.m_TmpDir=fc.getSelectedFile();
		tf1.setText(ConfigManager.m_TmpDir.toString());
	    }
	}
	else if (o==brw2){
	    fc=new JFileChooser(ConfigManager.m_PrjDir);
 	    fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY); //does not work well with JVM 1.3.x (works fine in 1.4)
	    returnVal= fc.showOpenDialog(this);
	    if (returnVal == JFileChooser.APPROVE_OPTION){
		ConfigManager.m_PrjDir=fc.getSelectedFile();
		tf2.setText(ConfigManager.m_PrjDir.toString());
	    }
	}
	else if (o==brw4){
	    fc=new JFileChooser(ConfigManager.m_DotPath);
	    fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
	    returnVal= fc.showOpenDialog(this);
	    if (returnVal == JFileChooser.APPROVE_OPTION){
		ConfigManager.m_DotPath=fc.getSelectedFile();
		tf4.setText(ConfigManager.m_DotPath.toString());
	    }
	}
	else if (o==brw3){
	    fc=new JFileChooser(ConfigManager.m_NeatoPath);
	    fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
	    returnVal= fc.showOpenDialog(this);
	    if (returnVal == JFileChooser.APPROVE_OPTION){
		ConfigManager.m_NeatoPath=fc.getSelectedFile();
		tf3.setText(ConfigManager.m_NeatoPath.toString());
	    }
	}
	else if (o==brw5){
	    fc=new JFileChooser(ConfigManager.m_GraphVizFontDir);
 	    fc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY); //does not work well with JVM 1.3.x (works fine in 1.4)
	    returnVal= fc.showOpenDialog(this);
	    if (returnVal == JFileChooser.APPROVE_OPTION){
		ConfigManager.m_GraphVizFontDir=fc.getSelectedFile();
		tf5.setText(ConfigManager.m_GraphVizFontDir.toString());
	    }
	}
        else if (o==brw6){
            fc=new JFileChooser(ConfigManager.m_Aaf2DotPath);
            fc.setFileSelectionMode(JFileChooser.FILES_ONLY);
            returnVal= fc.showOpenDialog(this);
            if (returnVal == JFileChooser.APPROVE_OPTION){
                ConfigManager.m_Aaf2DotPath=fc.getSelectedFile();
                tf6.setText(ConfigManager.m_Aaf2DotPath.toString());
            }
        }
	else if (o==cb1){
	    if (cb1.isSelected()){ConfigManager.DELETE_TEMP_FILES=true;}
	    else {ConfigManager.DELETE_TEMP_FILES=false;}
	}
	else if (o==okPrefs){updateVars();this.dispose();}
	else if (o==savePrefs){updateVars();application.saveConfiguration();}
	else if (o==antialiascb){
	    if (antialiascb.isSelected()){javax.swing.JOptionPane.showMessageDialog(this,Messages.antialiasingWarning);}
	    application.setAntialiasing(antialiascb.isSelected());
	}
    }

    void updateVars(){
	ConfigManager.SAVE_WINDOW_LAYOUT=saveWindowLayoutCb.isSelected();
        ConfigManager.CMD_LINE_OPTS=cmdLOptsTf.getText();
        ConfigManager.AAF2DOT_CMD_LINE_OPTS=dotECmdLOptsTf.getText();
// 	if (b1a.isSelected()){ConfigManager.GRAPH_ORIENTATION="LR";} else {ConfigManager.GRAPH_ORIENTATION="TB";}
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
