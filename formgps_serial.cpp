#include "formgps.h"
#include "qmlutil.h"

void FormGPS::autoSteerDataOutToPort()
{

}

void FormGPS::sendSteerSettingsOutAutoSteerPort()
{

}

void FormGPS::sectionControlOutToPort()
{

}

//called by the GPS delegate every time a chunk is rec'd
void FormGPS::SerialLineReceived(QByteArray sentence)
{
    //spit it out no matter what it says
    pn.rawBuffer.append(sentence);
    //recvSentenceSettings = sbNMEAFromGPS.ToString()
}

//serial port receive in its own thread
void FormGPS::sp_DataReceived()
{
    if (sp.isOpen())
    {
        //give it a sec to spit it out
        //System.Threading.Thread.Sleep(2000);

        //read whatever is in port
        QByteArray sentence = sp.readAll();
        SerialLineReceived(sentence);
    }
}

void FormGPS::SerialPortOpenGPS()
{
    //close it first
    SerialPortCloseGPS();

    if (sp.isOpen())
    {
        // TODO QAOG simulatorOnToolStripMenuItem.Checked = false;
        qmlItem(qml_root,"simSpeed")->setProperty("visible", false);
        qmlItem(qml_root,"simStopButton")->setProperty("visible", false);
        qmlItem(qml_root,"simSteer")->setProperty("visible", false);
        qmlItem(qml_root,"simSteerCenter")->setProperty("visible", false);

        // TODO QAOG Settings.Default.setMenu_isSimulatorOn = simulatorOnToolStripMenuItem.Checked;
        // TODO QAOG Settings.Default.Save();
    }


    if (!sp.isOpen())
    {
        sp.setPortName(portNameGPS);
        sp.setBaudRate(baudRateGPS);
        connect(&sp, SIGNAL(readyRead()), this, SLOT(sp_DataReceived()));
        // TODO QAOG sp.WriteTimeout = 1000;
    }

    if (!sp.open(QIODevice::ReadOnly))
    {
        //MessageBox.Show(exc.Message + "\n\r" + "\n\r" + "Go to Settings -> COM Ports to Fix", "No Serial Port Active");
        //WriteErrorLog("Open GPS Port " + e.ToString());

        //update port status labels
        //stripPortGPS.Text = " * * ";
        //stripPortGPS.ForeColor = Color.Red;
        //stripOnlineGPS.Value = 1;

        //SettingsPageOpen(0);
    }

    if (sp.isOpen())
    {
        //btnOpenSerial.Enabled = false;

        //discard any stuff in the buffers
        sp.clear(QSerialPort::AllDirections);

        //update port status label
        //stripPortGPS.Text = portNameGPS + " " + baudRateGPS.ToString();
        //stripPortGPS.ForeColor = Color.ForestGreen;

        // TODO QAOG Properties.Settings.Default.setPort_portNameGPS = portNameGPS;
        // TODO QAOG Properties.Settings.Default.setPort_baudRate = baudRateGPS;
        // TODO QAOG Properties.Settings.Default.Save();
    }
}

void FormGPS::SerialPortCloseGPS()
{
    //if (sp.IsOpen)
    {
        // TODO QAOG sp.DataReceived -= sp_DataReceived;
        sp.close();
        if (sp.error() != QSerialPort::NoError)
        {
            // TODO QAOG WriteErrorLog("Closing GPS Port" + e.ToString());
            // TODO QAOG MessageBox.Show(e.Message, "Connection already terminated?");
        }

        //update port status labels
        //stripPortGPS.Text = " * * " + baudRateGPS.ToString();
        //stripPortGPS.ForeColor = Color.ForestGreen;
        //stripOnlineGPS.Value = 1;
        // TODO QAOG sp.Dispose();
    }
}
