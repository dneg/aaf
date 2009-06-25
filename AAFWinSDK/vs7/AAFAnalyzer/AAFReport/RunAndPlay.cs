using System;
using System.Diagnostics;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Security.Principal;

 
namespace AAFReport
{
	/// <summary>
	///  Run:
	///   Spawning a program.
	/// 
	/// </summary>
	/// 
	public class Runner
	{ 
		[StructLayout(LayoutKind.Sequential)] public struct STARTUPINFO
		{
			public int cb;
			public String lpReserved;
			public String lpDesktop;
			public String lpTitle;
			public uint dwX;
			public uint dwY;
			public uint dwXSize;
			public uint dwYSize;
			public uint dwXCountChars;
			public uint dwYCountChars;
			public uint dwFillAttribute;
			public uint dwFlags;
			public short wShowWindow;
			public short cbReserved2;
			public IntPtr lpReserved2;
			public IntPtr hStdInput;
			public IntPtr hStdOutput;
			public IntPtr hStdError;
		}

		[StructLayout(LayoutKind.Sequential)] public struct PROCESS_INFORMATION
		{
			public IntPtr hProcess;
			public IntPtr hThread;
			public uint   dwProcessId;
			public uint   dwThreadId;
		}

		[StructLayout(LayoutKind.Sequential)] public struct SECURITY_ATTRIBUTES
		{
			public int    Length;
			public IntPtr lpSecurityDescriptor;
			public bool   bInheritHandle;
		}

		[DllImport("kernel32.dll", EntryPoint="CloseHandle", SetLastError=true, CharSet=CharSet.Auto, CallingConvention=CallingConvention.StdCall)]
		public extern static bool CloseHandle(IntPtr handle);

		[DllImport("Kernel32.dll", EntryPoint="CreateProcess", SetLastError=true, CharSet=CharSet.Ansi, CallingConvention=CallingConvention.StdCall)]
		public extern static bool CreateProcess(
			String lpApplicationName, 
			String lpCommandLine, 
			ref SECURITY_ATTRIBUTES lpProcessAttributes,
			ref SECURITY_ATTRIBUTES lpThreadAttributes, 
			bool bInheritHandle, 
			int dwCreationFlags, 
			IntPtr lpEnvironment,
			String lpCurrentDirectory, 
			ref STARTUPINFO lpStartupInfo, 
			out PROCESS_INFORMATION lpProcessInformation);

		static public void Run(out String status, out String identity, String cmdline, String startfrom)
		{
			bool      ret = false;

			identity = CurrentUserName;


			SECURITY_ATTRIBUTES sa  = new SECURITY_ATTRIBUTES();
			sa.bInheritHandle       = false;
			sa.Length               = Marshal.SizeOf(sa);
			sa.lpSecurityDescriptor = (IntPtr)0;

			STARTUPINFO si          = new STARTUPINFO();
			si.cb                   = Marshal.SizeOf(si);
			si.lpDesktop            = "";

			string commandLinePath;
			commandLinePath = cmdline;

			PROCESS_INFORMATION pi  = new PROCESS_INFORMATION();
			ret = CreateProcess(
				null,			//LPCTSTR lpApplicationName,
				cmdline,		//LPTSTR lpCommandLine,
				ref sa,			//LPSECURITY_ATTRIBUTES lpProcessAttributes,
				ref sa,			//LPSECURITY_ATTRIBUTES lpThreadAttributes,
				false,			//BOOL bInheritHandles,
				0,				//DWORD dwCreationFlags,
				(IntPtr)0,		//LPVOID lpEnvironment,
				startfrom,		//LPCTSTR lpCurrentDirectory,
				ref si,			//LPSTARTUPINFO lpStartupInfo,
				out pi);		//LPPROCESS_INFORMATION lpProcessInformation

			if (ret == false)
			{
				status = "CreateProcess failed with " + Marshal.GetLastWin32Error();
			}
			else
			{
				status = "CreateProcess SUCCESS.  The child PID is" + pi.dwProcessId;

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}
		}

		private static String m_UserName = "";
		static private String CurrentUserName
		{
			get
			{
				if (0 == m_UserName.Length)
				{
					WindowsIdentity wi = WindowsIdentity.GetCurrent();
					m_UserName = wi.Name.ToString();
				}
				return m_UserName;
			}
		}
		
	}
}
