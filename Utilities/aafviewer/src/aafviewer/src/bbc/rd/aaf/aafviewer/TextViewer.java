/*   FILE: TextViewer.java
 *   DATE OF CREATION:   Fri Dec 13 10:29:49 2002
 *   AUTHOR :            Emmanuel Pietriga (emmanuel@w3.org)
 *   MODIF:              Fri Jan 17 16:26:07 2003 by Emmanuel Pietriga
 *   Copyright (c) Emmanuel Pietriga, 2002. All Rights Reserved
 *   Licensed under the GNU LGPL. For full terms see the file COPYING.
 */

package bbc.rd.aaf.aafviewer;

import javax.swing.*;
import java.awt.event.*;
import javax.swing.event.*;
import java.awt.*;

/*A simple text viewer that displays the content of a stringbuffer. Can be set to automatically update its content periodically. Can be used for showing error logs, raw source files,...*/

public class TextViewer extends JFrame implements ActionListener,KeyListener,Runnable {

    Thread runView;

    JButton ok;

    StringBuffer text;
    int oldSize;

    int period;

    JTextArea ar;

    /**
     *@param msgs text displayed in main area
     *@param frameTitle string appearing in window title bar
     *@param d periodic update (in milliseconds) of the main area w.r.t msgs (0=no update)
     */
    public TextViewer(StringBuffer msgs,String frameTitle,int d){
	text=msgs;
	oldSize=text.length();
	period=d;
	ar=new JTextArea(text.toString());
	ar.setEditable(false);
	ar.setLineWrap(true);
	JScrollPane sp=new JScrollPane(ar);
	sp.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
	//sp.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
	Container cpane=this.getContentPane();
	GridBagLayout gridBag=new GridBagLayout();
	GridBagConstraints constraints=new GridBagConstraints();
	constraints.fill=GridBagConstraints.BOTH;
	constraints.anchor=GridBagConstraints.CENTER;
	cpane.setLayout(gridBag);
	buildConstraints(constraints,0,0,1,1,100,98);
	gridBag.setConstraints(sp,constraints);
	cpane.add(sp);
	ok=new JButton("OK");
	ok.addActionListener(this);ok.addKeyListener(this);
	constraints.anchor=GridBagConstraints.CENTER;
	constraints.fill=GridBagConstraints.HORIZONTAL;
	buildConstraints(constraints,0,1,1,1,100,2);
	gridBag.setConstraints(ok,constraints);
	cpane.add(ok);
	WindowListener w0=new WindowAdapter(){
		public void windowClosing(WindowEvent e){stop();dispose();}
	    };
	this.addWindowListener(w0);
	this.setTitle(frameTitle);
	this.pack();
	this.setSize(700,300);
	this.setVisible(true);
	ok.requestFocus();
	if (period>0){
	    addHierarchyListener(
		new HierarchyListener() {
		    public void hierarchyChanged(HierarchyEvent e) {
			if (isShowing()) {
			    start();
			} else {
			    stop();
			}
		    }
		}
		);
	    start();
	}
    }

    public void start() {
	runView = new Thread(this);
	runView.setPriority(Thread.MIN_PRIORITY);
	runView.start();
    }

    public synchronized void stop() {
	runView = null;
	notify();
    }

    public void run() {
	Thread me = Thread.currentThread();
 	while (runView==me){
	    if (text.length()!=oldSize){ar.setText(text.toString());}  //update text only if new error messages
	    oldSize=text.length();
	    try {
		runView.sleep(period);
	    }
	    catch (InterruptedException e) {}
 	}
    }


    public void actionPerformed(ActionEvent e){
	if (e.getSource()==ok){this.stop();this.dispose();}
    }

    public void keyPressed(KeyEvent e){
	if (e.getKeyCode()==KeyEvent.VK_ENTER){
	    if (e.getSource()==ok){this.stop();this.dispose();}
	}
    }

    public void keyReleased(KeyEvent e){}
    public void keyTyped(KeyEvent e){}

    void buildConstraints(GridBagConstraints gbc, int gx,int gy,int gw,int gh,int wx,int wy){
	gbc.gridx=gx;
	gbc.gridy=gy;
	gbc.gridwidth=gw;
	gbc.gridheight=gh;
	gbc.weightx=wx;
	gbc.weighty=wy;
    }

}

