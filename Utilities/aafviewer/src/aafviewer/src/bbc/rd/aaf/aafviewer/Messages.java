/*   FILE: Messages.java
 *   DATE OF CREATION:   Fri Jan 10 09:37:09 2003
 *   AUTHOR :            Emmanuel Pietriga (emmanuel@w3.org)
 *   MODIF:              Fri Jan 17 16:33:15 2003 by Emmanuel Pietriga
 *   Copyright (c) Emmanuel Pietriga, 2002. All Rights Reserved
 *   Licensed under the GNU LGPL. For full terms see the file COPYING.
 */



package bbc.rd.aaf.aafviewer;

public class Messages {

    /*warning, error, help and other messages*/

    static final String antialiasingWarning="Antialiasing requires additional computing resources.\nSetting it ON will noticeably reduce the refresh rate.";

    static final String pngOnlyIn140FirstPart="This functionality is only available when running IsaViz using a JVM version 1.4.0 or later (it requires the ImageIO API).\nIsaViz detected JVM version ";

    static final String pngOnlyIn140SecondPart="\nDo you want to proceed anyway (this will probably cause an error)?";

    static final String about="AAFViewer v0.1\nPhilip de Nier\nBBC Research and Development\n\n\nExtends 'ZGRViewer'\nAuthor: Emmanuel Pietriga\nhttp://sourceforge.net/projects/zvtm";

    static final String commands="Misc. Commands\n"
        +"* Ctrl+A = open a AAF file using aaf2dot\n"
	+"* Ctrl+D = open a file with dot\n"
        +"* Ctrl+N = open a file with neato\n"
	+"* Ctrl+P = Print current view\n"
	+"* Ctrl+Q = Exit AAFViewer\n"
        +"* G = get a global view (overview) of the graph\n"
        +"* B = go back one step\n"
// 	+"* Ctrl+F = toggle between standard and full screen exclusive mode\n"
// 	+"* Esc = exit full screen exclusive mode\n"
	+"\n"
	+"Navigation (left mouse button corresponds to the single button for Mac users)\n"
	+"* Press left or right mouse button and drag to move in the graph\n"
	+"* Hold Shift, press left or right mouse button and drag vertically to zoom-in/zoom-out\n"
        +"* Hold Ctrl, press left mouse button and drag to select a region of interest\n"
        +"* Click left mouse button on a node to select / deselect outgoing edges\n"
        +"* Hold Shift, click left mouse button on a node to select / deselect incoming edges\n"
        +"* Click right mouse button on a cluster or node to center on it\n"
	;

    static final String loadError="An error occured while loading file ";

}
