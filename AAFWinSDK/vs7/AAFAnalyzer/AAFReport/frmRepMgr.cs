using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;

namespace AAFReport
{
	/// <summary>
	/// Summary description for frmRepMgr.
	/// </summary>
	public class frmRepMgr : System.Windows.Forms.Form
	{
		private System.Windows.Forms.Button btnReport;
		private System.Windows.Forms.Panel pnlBrowsing;
		private System.Windows.Forms.Panel pnlAAFInfo;
		private System.Windows.Forms.Splitter sptBrowse_Info;
		private System.Windows.Forms.TreeView trvAAFFolder;
		private System.Windows.Forms.TreeView trvAAFFileInfo;
		private System.Windows.Forms.Timer tmrReportBuilder;
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.Button btnOpen;
		private System.Windows.Forms.Button btnBackUp;
		private System.Windows.Forms.Button btnView;
		private System.Windows.Forms.TextBox tbxDrive;
		private System.Windows.Forms.Button btnHelp;

		private bool m_bStartupHasReportTools = false;
			
		static private String m_AAFSuffix = ".aaf";
		static private String m_ZIPSuffix = ".zip";
		static private String m_ReportHTMLSuffix = "-report.html";
		static private String m_SummaryTXTSuffix = "-summary.txt";
		static private String m_StartupPath = "C:\\Program Files\\AAF Association Inc\\AAFAnalyzer";
		static private String m_SaltoPath = "C:\\Program Files\\DiskStream\\Salto";
		static private String m_WindowsPath = "C:\\Program Files\\Internet Explorer";

		private String m_CurrentAAFPath = "";
		private String m_CurrentAAFFile = "";
		private String m_CurrentZIPFile = "";
		
		public frmRepMgr()
		{
			//
			// Required for Windows Form Designer support
			//
			InitializeComponent();

			m_StartupPath = System.Environment.CurrentDirectory;
			
			m_bStartupHasReportTools = PathHasReportTools(m_StartupPath);
			if (!m_bStartupHasReportTools)
			{
				m_StartupPath = "C:\\Program Files\\AAF Association Inc\\AAFAnalyzer";
				m_bStartupHasReportTools = PathHasReportTools(m_StartupPath);
			}

			m_CurrentAAFPath = "d:\\Projects\\Bill\\AAF\\Gromit\\chicago-sample-files-oct06\\Adobe";
			System.IO.DirectoryInfo di = new System.IO.DirectoryInfo(m_CurrentAAFPath);
			if (!di.Exists)
			{
				if (m_bStartupHasReportTools)
					m_CurrentAAFPath = m_StartupPath;
				else
					m_CurrentAAFPath = "c:\\";
			}

			RefreshAndUpdate();
		}

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.Resources.ResourceManager resources = new System.Resources.ResourceManager(typeof(frmRepMgr));
			this.trvAAFFolder = new System.Windows.Forms.TreeView();
			this.btnReport = new System.Windows.Forms.Button();
			this.pnlBrowsing = new System.Windows.Forms.Panel();
			this.tbxDrive = new System.Windows.Forms.TextBox();
			this.btnOpen = new System.Windows.Forms.Button();
			this.btnBackUp = new System.Windows.Forms.Button();
			this.btnHelp = new System.Windows.Forms.Button();
			this.pnlAAFInfo = new System.Windows.Forms.Panel();
			this.btnView = new System.Windows.Forms.Button();
			this.trvAAFFileInfo = new System.Windows.Forms.TreeView();
			this.sptBrowse_Info = new System.Windows.Forms.Splitter();
			this.tmrReportBuilder = new System.Windows.Forms.Timer(this.components);
			this.pnlBrowsing.SuspendLayout();
			this.pnlAAFInfo.SuspendLayout();
			this.SuspendLayout();
			// 
			// trvAAFFolder
			// 
			this.trvAAFFolder.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.trvAAFFolder.ImageIndex = -1;
			this.trvAAFFolder.Location = new System.Drawing.Point(8, 40);
			this.trvAAFFolder.Name = "trvAAFFolder";
			this.trvAAFFolder.SelectedImageIndex = -1;
			this.trvAAFFolder.Size = new System.Drawing.Size(272, 336);
			this.trvAAFFolder.TabIndex = 4;
			this.trvAAFFolder.DoubleClick += new System.EventHandler(this.trvAAFFolder_DoubleClick);
			this.trvAAFFolder.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.trvBrowseAAF_AfterSelect);
			// 
			// btnReport
			// 
			this.btnReport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnReport.Location = new System.Drawing.Point(152, 384);
			this.btnReport.Name = "btnReport";
			this.btnReport.Size = new System.Drawing.Size(128, 32);
			this.btnReport.TabIndex = 3;
			this.btnReport.Text = "Report";
			this.btnReport.Click += new System.EventHandler(this.btnReport_Click);
			// 
			// pnlBrowsing
			// 
			this.pnlBrowsing.Controls.Add(this.tbxDrive);
			this.pnlBrowsing.Controls.Add(this.btnOpen);
			this.pnlBrowsing.Controls.Add(this.btnBackUp);
			this.pnlBrowsing.Controls.Add(this.trvAAFFolder);
			this.pnlBrowsing.Controls.Add(this.btnReport);
			this.pnlBrowsing.Controls.Add(this.btnHelp);
			this.pnlBrowsing.Dock = System.Windows.Forms.DockStyle.Left;
			this.pnlBrowsing.Location = new System.Drawing.Point(0, 0);
			this.pnlBrowsing.Name = "pnlBrowsing";
			this.pnlBrowsing.Size = new System.Drawing.Size(288, 421);
			this.pnlBrowsing.TabIndex = 5;
			// 
			// tbxDrive
			// 
			this.tbxDrive.Location = new System.Drawing.Point(96, 8);
			this.tbxDrive.Name = "tbxDrive";
			this.tbxDrive.Size = new System.Drawing.Size(56, 20);
			this.tbxDrive.TabIndex = 7;
			this.tbxDrive.Text = "c:\\";
			this.tbxDrive.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.tbxDrive_KeyPress);
			// 
			// btnOpen
			// 
			this.btnOpen.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.btnOpen.Location = new System.Drawing.Point(208, 8);
			this.btnOpen.Name = "btnOpen";
			this.btnOpen.TabIndex = 6;
			this.btnOpen.Text = "Open";
			this.btnOpen.Click += new System.EventHandler(this.btnOpen_Click);
			// 
			// btnBackUp
			// 
			this.btnBackUp.Location = new System.Drawing.Point(8, 8);
			this.btnBackUp.Name = "btnBackUp";
			this.btnBackUp.RightToLeft = System.Windows.Forms.RightToLeft.Yes;
			this.btnBackUp.TabIndex = 5;
			this.btnBackUp.Text = "Back Up";
			this.btnBackUp.Click += new System.EventHandler(this.btnBackUp_Click);
			// 
			// btnHelp
			// 
			this.btnHelp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.btnHelp.Location = new System.Drawing.Point(8, 384);
			this.btnHelp.Name = "btnHelp";
			this.btnHelp.Size = new System.Drawing.Size(128, 32);
			this.btnHelp.TabIndex = 3;
			this.btnHelp.Text = "Help";
			this.btnHelp.Click += new System.EventHandler(this.btnHelp_Click);
			// 
			// pnlAAFInfo
			// 
			this.pnlAAFInfo.Controls.Add(this.btnView);
			this.pnlAAFInfo.Controls.Add(this.trvAAFFileInfo);
			this.pnlAAFInfo.Dock = System.Windows.Forms.DockStyle.Fill;
			this.pnlAAFInfo.Location = new System.Drawing.Point(288, 0);
			this.pnlAAFInfo.Name = "pnlAAFInfo";
			this.pnlAAFInfo.Size = new System.Drawing.Size(416, 421);
			this.pnlAAFInfo.TabIndex = 6;
			// 
			// btnView
			// 
			this.btnView.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.btnView.Location = new System.Drawing.Point(336, 392);
			this.btnView.Name = "btnView";
			this.btnView.TabIndex = 1;
			this.btnView.Text = "View";
			this.btnView.Click += new System.EventHandler(this.btnView_Click);
			// 
			// trvAAFFileInfo
			// 
			this.trvAAFFileInfo.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
				| System.Windows.Forms.AnchorStyles.Left) 
				| System.Windows.Forms.AnchorStyles.Right)));
			this.trvAAFFileInfo.ImageIndex = -1;
			this.trvAAFFileInfo.Location = new System.Drawing.Point(16, 8);
			this.trvAAFFileInfo.Name = "trvAAFFileInfo";
			this.trvAAFFileInfo.SelectedImageIndex = -1;
			this.trvAAFFileInfo.Size = new System.Drawing.Size(392, 376);
			this.trvAAFFileInfo.TabIndex = 0;
			this.trvAAFFileInfo.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.trvAAFFileInfo_AfterSelect);
			// 
			// sptBrowse_Info
			// 
			this.sptBrowse_Info.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
			this.sptBrowse_Info.Location = new System.Drawing.Point(288, 0);
			this.sptBrowse_Info.Name = "sptBrowse_Info";
			this.sptBrowse_Info.Size = new System.Drawing.Size(6, 421);
			this.sptBrowse_Info.TabIndex = 7;
			this.sptBrowse_Info.TabStop = false;
			// 
			// tmrReportBuilder
			// 
			this.tmrReportBuilder.Tick += new System.EventHandler(this.tmrReportBuilder_Tick);
			// 
			// frmRepMgr
			// 
			this.AutoScaleBaseSize = new System.Drawing.Size(5, 13);
			this.ClientSize = new System.Drawing.Size(704, 421);
			this.Controls.Add(this.sptBrowse_Info);
			this.Controls.Add(this.pnlAAFInfo);
			this.Controls.Add(this.pnlBrowsing);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "frmRepMgr";
			this.Text = "Simple AAF Report Manager";
			this.pnlBrowsing.ResumeLayout(false);
			this.pnlAAFInfo.ResumeLayout(false);
			this.ResumeLayout(false);

		}
		#endregion

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main() 
		{
			Application.Run(new frmRepMgr());
		}

		private bool PathHasReportTools(String strPath)
		{
			bool	ret = false;

			if (System.IO.File.Exists(strPath + "\\" + "AAFAnalyzerMain.exe"))
			{
				if (System.IO.File.Exists(strPath + "\\" + "python.exe"))
				{
					if (System.IO.File.Exists(strPath+ "\\" + "AAFRequirements.xml"))
					{
						ret = true;
					}
				}
			}

			return ret;
		}

		private void trvAAFFolder_Update()
		{
			String[] asFiles = null;
			String[] asDirs = null;
			try
			{
				asFiles = System.IO.Directory.GetFiles(m_CurrentAAFPath);
				asDirs = System.IO.Directory.GetDirectories(m_CurrentAAFPath);
			}
			catch (System.IO.IOException ioe)
			{
				MessageBox.Show(ioe.Message);
			}
			trvAAFFolder.BeginUpdate();
			trvAAFFolder.Nodes.Clear();
			TreeNode nFolder = trvAAFFolder.Nodes.Add(m_CurrentAAFPath);
			if (asFiles != null)
			{
				foreach(string strFile in asFiles) 
				{
					if (strFile.IndexOf(m_AAFSuffix) == strFile.Length - m_AAFSuffix.Length ||
						strFile.IndexOf(m_ZIPSuffix) == strFile.Length - m_ZIPSuffix.Length)
					{
						System.IO.FileInfo f = new System.IO.FileInfo(strFile);
						nFolder.Nodes.Add(f.Name);
					}
				}
			}
			if (asDirs != null)
			{
				foreach(string strDir in asDirs) 
				{
					System.IO.DirectoryInfo di = new System.IO.DirectoryInfo(strDir);
					if (IsViewableDirectory(di))
						nFolder.Nodes.Add(di.Name);
				}
			}
			trvAAFFolder.EndUpdate();
			trvAAFFolder.ExpandAll();
		}

		private void btnReport_Update()
		{
			btnReport.Enabled = m_bStartupHasReportTools && m_CurrentAAFFile.Length > 0;
		}

		private void trvBrowseAAF_AfterSelect(object sender, System.Windows.Forms.TreeViewEventArgs e)
		{
			string f = e.Node.Text;
			if (f.IndexOf(m_AAFSuffix) == f.Length - m_AAFSuffix.Length)
				m_CurrentAAFFile = f;
			else
				m_CurrentAAFFile = "";
			if (f.IndexOf(m_ZIPSuffix) == f.Length - m_ZIPSuffix.Length)
				m_CurrentZIPFile = f;
			else
				m_CurrentZIPFile = "";
			trvAAFFileInfo_Update();
			btnReport_Update();
			NavButtonsUpdate();
		}

		private String StrMinus(String op1, String op2)
		{
			String ret = op1;

			int i2 = op1.IndexOf(op2);
			if (i2 >= 0)
				ret = op1.Substring(0, i2) + op1.Substring(i2 + op2.Length, op1.Length - (i2 + op2.Length));
			
			return ret;
		}

		private String BuildCurrentAAFFileSpec()
		{
			String ret = m_CurrentAAFPath + "\\" + m_CurrentAAFFile;
			return ret;
		}
		
		private String BuildCurrentAAFHTMLReportFileSpec()
		{
			String ret = StrMinus(BuildCurrentAAFFileSpec(), m_AAFSuffix) + m_ReportHTMLSuffix;
			return ret;
		}

		private String BuildCurrentAAFSummaryTextReportFileSpec()
		{
			String ret = StrMinus(BuildCurrentAAFFileSpec(), m_AAFSuffix) + m_SummaryTXTSuffix;
			return ret;
		}

		private void trvAAFFileInfo_Update()
		{
			trvAAFFileInfo.BeginUpdate();
			trvAAFFileInfo.Nodes.Clear();
			
			if (0 < m_CurrentZIPFile.Length)
			{
				trvAAFFileInfo.Nodes.Add("Information on ZIP files not supported!");
			}
			else if (0 == m_CurrentAAFFile.Length)
			{
				trvAAFFileInfo.Nodes.Add("Not an AAF file!");
			}
			else
			{
				System.IO.FileInfo fAAF = new System.IO.FileInfo(BuildCurrentAAFFileSpec());
				TreeNode nAAF = trvAAFFileInfo.Nodes.Add(fAAF.Name);
				nAAF.Nodes.Add("Created: " + fAAF.CreationTime);
				nAAF.Nodes.Add("Size: " + fAAF.Length);
			
				System.IO.FileInfo fHTML = new System.IO.FileInfo(BuildCurrentAAFHTMLReportFileSpec());
				TreeNode nHTML;
				if (fHTML.Exists)
				{
					nHTML = trvAAFFileInfo.Nodes.Add("HTML Report: " + fHTML.Name);
					nHTML.Nodes.Add("Created: " + fHTML.CreationTime);
					nHTML.Nodes.Add("Size: " + fHTML.Length);
				}
				else
				{
					nHTML = trvAAFFileInfo.Nodes.Add("HTML Report: None");
				}

				System.IO.FileInfo fTXT = new System.IO.FileInfo(BuildCurrentAAFSummaryTextReportFileSpec());
				TreeNode nTXT;
				if (fTXT.Exists)
				{
					nTXT = trvAAFFileInfo.Nodes.Add("Summary Text Report: " +fTXT.Name);
					nTXT.Nodes.Add("Created: " + fTXT.CreationTime);
					nTXT.Nodes.Add("Size: " + fTXT.Length);
				}
				else
				{
					nTXT  = trvAAFFileInfo.Nodes.Add("Summary Text Report: None");
				}
			}
			trvAAFFileInfo.EndUpdate();
			trvAAFFileInfo.ExpandAll();
		}

		private void btnReport_Click(object sender, System.EventArgs e)
		{
			String status;
			String identity;
			String cmdline;

			cmdline = "\"" + m_StartupPath + "\\python.exe\" analyzerhtml.py \"" + BuildCurrentAAFSummaryTextReportFileSpec() + "\" \"" + BuildCurrentAAFHTMLReportFileSpec() + "\" AAFAnalyzerMain.exe \"" + BuildCurrentAAFFileSpec() + "\" AAFRequirements.xml ";
			Runner.Run(out status, out identity, cmdline, m_StartupPath);

			trvAAFFileInfo_Update();
			tmrReportBuilder.Interval = 1000;
			tmrReportBuilder.Start();
		}

		private int m_MaxTicks = 5;
		private int m_nCurrTicks = 0;
		private void tmrReportBuilder_Tick(object sender, System.EventArgs e)
		{
			trvAAFFileInfo_Update();
			m_nCurrTicks++;
			if (m_nCurrTicks == m_MaxTicks)
			{
				tmrReportBuilder.Stop();
				m_nCurrTicks = 0;
			}
		}

		private void RefreshAndUpdate()
		{
			m_CurrentAAFFile = "";
			m_CurrentZIPFile = "";
			tbxDrive_SyncToCurrentAAFPath();
			trvAAFFileInfo_Update();
			trvAAFFolder_Update();
			btnReport_Update();
			NavButtonsUpdate();
			ViewButtonUpdate();
		}

		private bool SaltoIsInstalled()
		{
			bool	ret = false;

			ret = System.IO.File.Exists(m_SaltoPath + "\\salto.exe");

			return ret;
		}

		private void ViewButtonUpdate()
		{
			if (trvAAFFileInfo.SelectedNode != null)
			{
				String strNode = StrMinus(trvAAFFileInfo.SelectedNode.ToString(), "TreeNode: ");
				if (strNode.IndexOf(m_AAFSuffix) == strNode.Length - m_AAFSuffix.Length)
					btnView.Enabled = System.IO.File.Exists(BuildCurrentAAFFileSpec()) && SaltoIsInstalled();
				else if (strNode.IndexOf(m_ReportHTMLSuffix) == strNode.Length - m_ReportHTMLSuffix.Length)
					btnView.Enabled = System.IO.File.Exists(BuildCurrentAAFHTMLReportFileSpec());
				else if (strNode.IndexOf(m_SummaryTXTSuffix) == strNode.Length - m_SummaryTXTSuffix.Length)
					btnView.Enabled = System.IO.File.Exists(BuildCurrentAAFSummaryTextReportFileSpec());
				else
					btnView.Enabled = false;
			}
			else
			{
				btnView.Enabled = false;
			}

		}

		private void NavButtonsUpdate()
		{
			System.IO.DirectoryInfo di = new System.IO.DirectoryInfo(m_CurrentAAFPath);
			btnBackUp.Enabled = (di.Parent != null);
			
			bool b_DirSelected = false;
			if (trvAAFFolder.SelectedNode != null)
			{
				String strNode = StrMinus(trvAAFFolder.SelectedNode.ToString(), "TreeNode: ");
				if (strNode.IndexOf(":\\") == 1)
				{
					// This is a path node (current path) no navigation needed
				}
				else
				{
					try
					{
						di = new System.IO.DirectoryInfo(m_CurrentAAFPath + "\\" + strNode);
						b_DirSelected = IsViewableDirectory(di);
					}
					catch (System.NotSupportedException nse)
					{
						// Additional information: The given path's format is not supported.
						int hc = nse.GetHashCode(); // hc is 20 or 22 for above message, other than that ??
					}
				}
			}
			btnOpen.Enabled = b_DirSelected;
		}

		private void btnBackUp_Click(object sender, System.EventArgs e)
		{
			System.IO.DirectoryInfo di = new System.IO.DirectoryInfo(m_CurrentAAFPath);
			if (di.Parent != null)
			{
				m_CurrentAAFPath = di.Parent.FullName;
				RefreshAndUpdate();
			}
		}

		private void trvAAFFolder_Open()
		{
			if (trvAAFFolder.SelectedNode != null)
			{
				String strNode = StrMinus(trvAAFFolder.SelectedNode.ToString(), "TreeNode: ");
				if (strNode.IndexOf(":\\") == 1)
				{
					// This is a path node (current path) no navigation needed
				}
				else
				{
					try
					{
						System.IO.DirectoryInfo di = new System.IO.DirectoryInfo(m_CurrentAAFPath + "\\" + strNode);
						if (IsViewableDirectory(di))
						{
							m_CurrentAAFPath = di.FullName;
							RefreshAndUpdate();
						}
					}
					catch (System.NotSupportedException nse)
					{
						// Additional information: The given path's format is not supported.
						int hc = nse.GetHashCode(); // hc is 20 or 22 for above message, other than that ??
					}
				}
			}
		}

		private void btnOpen_Click(object sender, System.EventArgs e)
		{
			trvAAFFolder_Open();
		}

		private void trvAAFFolder_DoubleClick(object sender, System.EventArgs e)
		{
			trvAAFFolder_Open();
		}

		private void ViewAAFFile(String strFile)
		{
			String status;
			String identity;
			String cmdline;

			cmdline = "\"" + m_SaltoPath + "\\salto.exe\" \"" + strFile + "\"";
			Runner.Run(out status, out identity, cmdline, m_SaltoPath);
		}

		private void ViewHTMLFile(String strFile)
		{
			String status;
			String identity;
			String cmdline;

			cmdline = "Explorer \"" + strFile + "\"";
			Runner.Run(out status, out identity, cmdline, m_WindowsPath );
		}

		private void ViewTXTFile(String strFile)
		{
			String status;
			String identity;
			String cmdline;

			cmdline = "notepad.exe \"" + strFile + "\"";
			Runner.Run(out status, out identity, cmdline, m_StartupPath);
		}

		private void btnView_Click(object sender, System.EventArgs e)
		{
			if (trvAAFFileInfo.SelectedNode != null)
			{
				String strNode = StrMinus(trvAAFFileInfo.SelectedNode.ToString(), "TreeNode: ");
				if (strNode.IndexOf(m_AAFSuffix) == strNode.Length - m_AAFSuffix.Length)
					ViewAAFFile(BuildCurrentAAFFileSpec());
				else if (strNode.IndexOf(m_ReportHTMLSuffix) == strNode.Length - m_ReportHTMLSuffix.Length)
					ViewHTMLFile(BuildCurrentAAFHTMLReportFileSpec());
				else if (strNode.IndexOf(m_SummaryTXTSuffix) == strNode.Length - m_SummaryTXTSuffix.Length)
					ViewTXTFile(BuildCurrentAAFSummaryTextReportFileSpec());
			}

		}

		private void trvAAFFileInfo_AfterSelect(object sender, System.Windows.Forms.TreeViewEventArgs e)
		{
			ViewButtonUpdate();
			tmrReportBuilder.Stop();
			m_nCurrTicks = 0;
		}

		private void tbxDrive_KeyPress(object sender, System.Windows.Forms.KeyPressEventArgs e)
		{
			String strDriveText = GetDriveText(e.KeyChar);
			if (strDriveText.Length > 0)
			{
				tbxDrive.Text = strDriveText;
				m_CurrentAAFPath = tbxDrive.Text;
				trvAAFFolder_Update();
			}
			e.Handled = true;
		}

		private void tbxDrive_SyncToCurrentAAFPath()
		{
			String strDriveText = GetDriveText(m_CurrentAAFPath[0]);
			if (strDriveText.Length > 0)
				tbxDrive.Text = strDriveText;
		}

		private String GetDriveText(char chAny)
		{
			String	ret = "";

			char	chKey = ' ';
			if (chAny >= 'a' && chAny <= 'z')
				chKey = (char)(chAny - 'a' + 'A');
			else if (chAny >= 'A' && chAny <= 'Z')
				chKey = chAny;
			if (chKey > ' ')
				ret = Convert.ToString(chKey) + ":\\";

			return ret;
		}

		private void btnHelp_Click(object sender, System.EventArgs e)
		{
			ViewHTMLFile("\"" + m_StartupPath + "\\ReadMe.html\"");
		}
		
		private bool IsViewableDirectory(System.IO.DirectoryInfo di)
		{
			bool	ret = false;

			if (di.Exists &&
				System.IO.FileAttributes.Directory == System.IO.FileAttributes.Directory ||
				System.IO.FileAttributes.Directory == (System.IO.FileAttributes.Directory | System.IO.FileAttributes.ReadOnly))
			{
				ret = true;
			}
			
			return ret;
		}
		
	}

}
