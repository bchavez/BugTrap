using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Text;
using BugTrapServer;

public class Global : System.Web.HttpApplication
{
    static private string GetReportNamePrefix(ApplicationSettings applicationSettings)
    {
        StringBuilder reportNamePrefix = new StringBuilder();
        string reportName = applicationSettings.ReportName;
        for (int index = 0; index < reportName.Length; ++index)
        {
            char ch = reportName[index];
            if (ch == '%')
                break;
            reportNamePrefix.Append(ch);
        }
        return reportNamePrefix.ToString();
    }

    static private bool GetNextToken(ApplicationSettings applicationSettings, ref int index, out char token)
    {
        token = '\0';
        string reportName = applicationSettings.ReportName;
        if (index >= reportName.Length)
            return false;
        while (index < reportName.Length)
        {
            char ch = reportName[index];
            if (ch == '%')
            {
                if (++index >= reportName.Length)
                    break;
                ch = reportName[index++];
                token = ch;
                return true;
            }
            ++index;
        }
        return true;
    }

    static private bool AutoNumberReports(ApplicationSettings applicationSettings)
    {
        int index = 0;
        char token;
        while (GetNextToken(applicationSettings, ref index, out token))
        {
            if (token == 'n')
                return true;
        }
        return false;
    }

    static private int GetReportNumber(ApplicationSettings applicationSettings, string fileName)
    {
        int dataIndex = 0, refIndex = 0;
        for (; ; )
        {
            char token;
            int refStart = refIndex;
            if (!GetNextToken(applicationSettings, ref refIndex, out token))
                break;
            int length = refIndex - refStart;
            if (token == 'n' || token == 't')
                length -= 2;
            else
                length -= 1;
            dataIndex += length;
            if (token == 'n' || token == 't')
            {
                int dataStart = dataIndex;
                while (dataIndex < fileName.Length && char.IsDigit(fileName[dataIndex]))
                    ++dataIndex;
                if (token == 'n')
                {
                    int dataLength = dataIndex - dataStart;
                    if (dataLength > 0)
                        return int.Parse(fileName.Substring(dataStart, dataLength));
                }
            }
        }
        return -1;
    }

    static public string GetReportName(ApplicationSettings applicationSettings, int reportNumber, string extension)
    {
        StringBuilder reportName = new StringBuilder();
        int index = 0;
        for (; ; )
        {
            char token;
            int start = index;
            if (!GetNextToken(applicationSettings, ref index, out token))
                break;
            int length = index - start;
            if (token != '\0')
                length -= 2;
            reportName.Append(applicationSettings.ReportName, start, length);
            switch (token)
            {
                case 'n':
                    reportName.Append(reportNumber);
                    break;
                case 't':
                    reportName.Append(DateTime.Now.Ticks);
                    break;
                default:
                    if (token != '\0')
                        reportName.Append(token);
                    break;
            }
        }
        reportName.Append('.');
        reportName.Append(extension);
        return reportName.ToString();
    }

    /// <summary>
    /// Load directories contents from the disk.
    /// </summary>
    /// <param name="applicationSettings">Application settings.</param>
    /// <return>Unique numbers of recent bug reports.</return>
    static private Dictionary<string, AppDirInfo> LoadDirectories(ApplicationSettings applicationSettings)
    {
        Dictionary<string, AppDirInfo> lastReportNumbers = new Dictionary<string, AppDirInfo>();
        Directory.CreateDirectory(applicationSettings.ReportPath);
        string[] directories = Directory.GetDirectories(applicationSettings.ReportPath);
        string reportNamePrefix = GetReportNamePrefix(applicationSettings);
        string reportNameFilter = reportNamePrefix + '*';
        bool autoNumberReports = AutoNumberReports(applicationSettings);
        foreach (string directory in directories)
        {
            string dirName = Path.GetFileName(directory);
            AppDirInfo appDirInfo = new AppDirInfo();
            string[] files = Directory.GetFiles(directory, reportNameFilter);
            foreach (string file in files)
            {
                string fileName = Path.GetFileName(file);
                if (!fileName.StartsWith(reportNamePrefix))
                    continue;
                ++appDirInfo.NumReports;
                if (autoNumberReports)
                {
                    int reportNumber = GetReportNumber(applicationSettings, fileName);
                    if (appDirInfo.MaxReportNumber < reportNumber)
                        appDirInfo.MaxReportNumber = reportNumber;
                }
                else
                    appDirInfo.MaxReportNumber = appDirInfo.NumReports;
            }
            lastReportNumbers[dirName] = appDirInfo;
        }
        return lastReportNumbers;
    }

    /// <summary>
    /// Code that runs on application startup.
    /// </summary>
    /// <param name="sender">Event souurce.</param>
    /// <param name="e">Event data.</param>
    [DoNotObfuscate]
    void Application_Start(object sender, EventArgs e)
    {
        ApplicationSettings applicationSettings = (ApplicationSettings)ConfigurationManager.GetSection("applicationSettings");
        if (!Path.IsPathRooted(applicationSettings.ReportPath))
            applicationSettings.ReportPath = this.Server.MapPath(applicationSettings.ReportPath);
        this.Application["applicationSettings"] = applicationSettings;
        this.Application["lastReportNumbers"] = LoadDirectories(applicationSettings);
    }

    /// <summary>
    /// Code that runs on application shutdown.
    /// </summary>
    /// <param name="sender">Event souurce.</param>
    /// <param name="e">Event data.</param>
    [DoNotObfuscate]
    void Application_End(object sender, EventArgs e)
    {
    }

    /// <summary>
    /// Code that runs when an unhandled error occurs.
    /// </summary>
    /// <param name="sender">Event souurce.</param>
    /// <param name="e">Event data.</param>
    [DoNotObfuscate]
    void Application_Error(object sender, EventArgs e)
    {
    }

    /// <summary>
    /// Code that runs when a new session is started.
    /// </summary>
    /// <param name="sender">Event souurce.</param>
    /// <param name="e">Event data.</param>
    [DoNotObfuscate]
    void Session_Start(object sender, EventArgs e)
    {
    }

    /// <summary>
    /// Code that runs when a session ends.
    /// Note: The Session_End event is raised only when the sessionstate mode
    /// is set to InProc in the Web.config file. If session mode is set to StateServer
    /// or SQLServer, the event is not raised.
    /// </summary>
    /// <param name="sender">Event souurce.</param>
    /// <param name="e">Event data.</param>
    [DoNotObfuscate]
    void Session_End(object sender, EventArgs e)
    {
    }
}
