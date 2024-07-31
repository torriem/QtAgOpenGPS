#include "formudp.h"
#include <QNetworkInterface>


FormUDP::FormUDP(QObject *parent)
    : QObject(parent), mf(nullptr)//initialize mf to nullptr
{
    //initialization if needee
}
void FormUDP::SetFormLoop(FormLoop *formLoop){
    mf = formLoop; //set the FormLoop reference
}

void FormUDP::FormUDp_Load(){
    mf->ipAutoSet[0] = 99;
    mf->ipAutoSet[1] = 99;
    mf->ipAutoSet[2] = 99;

    //lblHostname.Text = Dns.GetHostName(); // Retrieve the Name of HOST

    /*lblNetworkHelp.Text =
  Properties.Settings.Default.etIP_SubnetOne.ToString() + " . " +
  Properties.Settings.Default.etIP_SubnetTwo.ToString() + " . " +
  Properties.Settings.Default.etIP_SubnetThree.ToString();*/

    //nudFirstIP.Value = ipNew[0] = ipCurrent[0] = Properties.Settings.Default.etIP_SubnetOne;
    //nudSecndIP.Value = ipNew[1] = ipCurrent[1] = Properties.Settings.Default.etIP_SubnetTwo;
    //nudThirdIP.Value = ipNew[2] = ipCurrent[2] = Properties.Settings.Default.etIP_SubnetThree;

    ScanNetwork();
}
int tickCounter = 0;

void FormUDP::timer1_Tick()
{
    if (!mf->scanReply.isNewData)
    {
        mf->ipAutoSet[0] = 99;
        mf->ipAutoSet[1] = 99;
        mf->ipAutoSet[2] = 99;
        //btnAutoSet.Enabled = false;
    }
    else
    {
        //btnAutoSet.Enabled = true;
    }

    if (mf->scanReply.isNewSteer)
    {
        //lblSteerIP.Text = mf.scanReply.steerIP;
        mf->scanReply.isNewSteer = false;
        //lblNewSubnet.Text = mf.scanReply.subnetStr;
    }

    if (mf->scanReply.isNewMachine)
    {
        //lblMachineIP.Text = mf                    }
        //mf.scanReply.machineIP; //should this be commented out???
        mf->scanReply.isNewMachine = false;
        //lblNewSubnet.Text = mf.scanReply.subnetStr;
    }

    if (mf->scanReply.isNewIMU)
    {
        //lblIMU_IP.Text = mf.scanReply.IMU_IP;
        mf->scanReply.isNewIMU = false;
        //lblNewSubnet.Text = mf.scanReply.subnetStr;
    }

    if (mf->scanReply.isNewGPS)
    {
        //lblGPSIP.Text = mf.scanReply.GPS_IP;
        mf->scanReply.isNewGPS = false;
        //lblNewSubnet.Text = mf.scanReply.subnetStr;
    }

    if (tickCounter == 4)
    {
        /* I think this is for the boxes that show which modules are connected
         * in the UDP config page. I'll  comment out for now
         *
		if (mf.btnSteer.BackColor == Color.LimeGreen) lblBtnSteer.BackColor = Color.LimeGreen;
		else lblBtnSteer.BackColor = Color.Red;

		if (mf.btnMachine.BackColor == Color.LimeGreen) lblBtnMachine.BackColor = Color.LimeGreen;
		else lblBtnMachine.BackColor = Color.Red;

		if (mf.btnGPS.BackColor == Color.LimeGreen) lblBtnGPS.BackColor = Color.LimeGreen;
		else lblBtnGPS.BackColor = Color.Red;

		if (mf.btnIMU.BackColor == Color.LimeGreen) lblBtnIMU.BackColor = Color.LimeGreen;
        else lblBtnIMU.BackColor = Color.Red;*/
    }

    if (tickCounter > 5)
    {
        ScanNetwork();
        tickCounter = 0;
        //lblSubTimer.Text = "Scanning";
    }
    else
    {
        //lblSubTimer.Text = "-";
    }
    tickCounter++;

}

void FormUDP::ScanNetwork()
{

    //tboxNets.Text = "";

    //lblSteerIP.Text = lblMachineIP.Text = lblGPSIP.Text = lblIMU_IP.Text = lblNewSubnet.Text = "";
    mf->scanReply.isNewData = false;

    //bool isSubnetMatchCard = false;

    //QByteArray scanModules = { 0x80, 0x81, 0x7F, 202, 3, 202, 202, 5, 0x47 };
    QByteArray scanModules = QByteArray::fromRawData("\x80\x81\x7F\xCA\x03\xCA\xCA\x05\x47", 9);

    //I really don't want to think about how little I understand this, and how much AI did. If anything wrong, please fix
    //Send out 255x4 to each installed network interface
    for (const QNetworkInterface& nic : QNetworkInterface::allInterfaces())
    {
        if (nic.flags().testFlag(QNetworkInterface::IsUp) && nic.flags().testFlag(QNetworkInterface::IsRunning))
        {
            for (const QNetworkAddressEntry& info : nic.addressEntries())
            {
                // Only InterNetwork and not loopback which have a subnetmask
                if (info.ip().protocol() == QAbstractSocket::IPv4Protocol && !info.ip().isLoopback())
                {
                    QUdpSocket* scanSocket = new QUdpSocket(this);
                    //Socket scanSocket;

                    try
                    {
                        //create list of interface properties
                        //all UI stuff
                        if ((nic.flags().testFlag(QNetworkInterface::IsUp)))
                        {
                            //var properties = nic.GetIPStatistics();
                            //tboxNets.Text +=
                            //	info.Address + "  - " + nic.OperationalStatus + "\r\n";

                            //tboxNets.Text += nic.Name.ToString() + "\r\n";
                            /*tboxNets.Text += "Sent: " + (properties.NonUnicastPacketsSent
									+ properties.UnicastPacketsSent).ToString()
								+ "   Recd: " + (properties.NonUnicastPacketsReceived
                                        + properties.UnicastPacketsReceived).ToString() + "\r\n\r\n";*/
                        }

                        if (nic.flags().testFlag(QNetworkInterface::IsUp)
                            //&& !info.ip().netmask().isNull())
                            && nic.flags().testFlag(QNetworkInterface::IsRunning)
                            && !info.netmask().isNull())
                        {
                            QByteArray data = info.ip().toString().toUtf8();
                            if (data[0] == ipCurrent[0] && data[1] == ipCurrent[1] && data[2] == ipCurrent[2])
                            {
                                //isSubnetMatchCard = true;
                            }

                            //send scan reply out each network interface
                            /*Did I set the options all right? David
                             * scanSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, true);
                            scanSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
                            scanSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.DontRoute, true);*/

                            //scanSocket->setSocketOption(QAbstractSocket::MulticastLoopbackOption, true);
                            //scanSocket->setSocketOption(QAbstractSocket::ReuseAddressHint, true);
                            //scanSocket->setSocketOption(QAbstractSocket::ReuseAddressHint, true);
                            //this sets up the connection, and checks if it worked
                            if (scanSocket->bind(mf->ethModulesSet.address, mf->ethModulesSet.portToListen))
                                scanSocket->writeDatagram(scanModules, QHostAddress(mf->ethModulesSet.address), mf->ethModulesSet.portToSend);
                            else
                                qDebug() << "ScanSocket error: " << scanSocket->errorString();
                        }
                    }
                    catch (const std::exception& ex)
                    {
                        qDebug() << "Bind Error = " << ex.what();
                    }
                    delete scanSocket;
                }
            }
        }
    }

    /* GUI code
	if (isSubnetMatchCard)
	{
		lblNetworkHelp.BackColor = System.Drawing.Color.LightGreen;
		lblNoAdapter.Visible = false;
	}
	else
	{
		lblNetworkHelp.BackColor = System.Drawing.Color.Salmon;
		lblNoAdapter.Visible = true;
    }*/
}

void FormUDP::btnSendSubnet_Click()
{

    sendIPToModules[7] = ipNew[0];
    sendIPToModules[8] = ipNew[1];
    sendIPToModules[9] = ipNew[2];

    //loop thru all interfaces
    for (const QNetworkInterface& nic : QNetworkInterface::allInterfaces())
    {
                        if (nic.flags().testFlag(QNetworkInterface::IsUp))
            {
                for (const QNetworkAddressEntry& info : nic.addressEntries())
                {
                    //if (info.ip().protocol() == QAbstractSocket::IPv4Protocol && !info.ip().isLoopback() && !info.ip().netmask().isNull())
                    if (info.ip().protocol() == QAbstractSocket::IPv4Protocol && !info.ip().isLoopback())
                    {
                        QUdpSocket* scanSocket = new QUdpSocket(this);
                        try
                        {
                            if (nic.flags().testFlag(QNetworkInterface::IsUp) && !info.netmask().isNull())
                            {
                                /*scanSocket->setSocketOption(QAbstractSocket::Broadcast, true);
                                scanSocket->setSocketOption(QAbstractSocket::ReuseAddress, true);
                                scanSocket->setSocketOption(QAbstractSocket::DontRoute, true);*/

                                scanSocket->bind(QHostAddress(info.ip()), 9999);
                                scanSocket->writeDatagram(sendIPToModules, QHostAddress(mf->ethModulesSet.address), mf->ethModulesSet.portToSend);
                            }
                        }
                        catch (const std::exception& ex)
                        {
                            qDebug() << "nic Loop =" << ex.what();
                        }
                        delete scanSocket;
                    }
                }
        }
    }

    settings.value("UDPComm/IP1") = ipCurrent[0] = ipNew[0];
    settings.value("UDPComm/IP2") = ipCurrent[1] = ipNew[1];
    settings.value("UDPComm/IP3") = ipCurrent[2] = ipNew[2];

    //Properties.Settings.Default.Save();
    //does the lower replace the upper? Remove if it does
    settings.sync();

    // update ethUDP from settings
    uint ip1 = settings.value("UDPComm/IP1").toUInt();
    uint ip2 = settings.value("UDPComm/IP2").toUInt();
    uint ip3 = settings.value("UDPComm/IP3").toUInt();
    uint ip4 = 255; //broadcast

    quint32 ipAddress = (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;

    mf->ethUDP.address.setAddress(ipAddress);

    /*lblNetworkHelp.Text =
		Properties.Settings.Default.etIP_SubnetOne.ToString() + " . " +
		Properties.Settings.Default.etIP_SubnetTwo.ToString() + " . " +
        Properties.Settings.Default.etIP_SubnetThree.ToString();*/
}

//pboxSendSteer.Visible = false;
//btnSerialCancel.Image = Properties.Resources.back_button;
//}

void FormUDP::btnAutoSet_Click()
{

    //nudFirstIP.Value = mf.scanReply.subnet[0];
    //nudSecndIP.Value = mf.scanReply.subnet[1];
    //nudThirdIP.Value = mf.scanReply.subnet[2];
    ipNew[0] = mf->scanReply.subnet[0];
    ipNew[1] = mf->scanReply.subnet[1];
    ipNew[2] = mf->scanReply.subnet[2];
    //btnSerialCancel.Image = Properties.Resources.Cancel64;
    //pboxSendSteer.Visible = true;
}
/*This is all UI. Don't know how much we'll reuse
void FormUDP::nudFirstIP_Click()
{
    ipNew[0] = (byte)nudFirstIP.Value;
    ipNew[1] = (byte)nudSecndIP.Value;
    ipNew[2] = (byte)nudThirdIP.Value;
    btnSendSubnet.Enabled = true;
	pboxSendSteer.Visible = true;
    btnSerialCancel.Image = Properties.Resources.Cancel64;
}*/
