#include "formloop.h"
#include "agioproperty.h"
#include <QNetworkInterface>

void FormLoop::FormUDp_Load(){
    ipCurrent.append("192"); //set ipCurrent to hold something at least
    ipCurrent.append("168");
    ipCurrent.append("5");

    ipNew.append("192");
    ipNew.append("168");
    ipNew.append("5");

    ipAutoSet.resize(3,0);

    ipAutoSet[0] = 99;
    ipAutoSet[1] = 99;
    ipAutoSet[2] = 99;

    ethModulesSet.portToListen = ethUDP.portToListen;
    ethModulesSet.portToSend = ethUDP.portToSend;

    //lblHostname.Text = Dns.GetHostName(); // Retrieve the Name of HOST

    /*lblNetworkHelp.Text =
  Properties.Settings.Default.etIP_SubnetOne.ToString() + " . " +
  Properties.Settings.Default.etIP_SubnetTwo.ToString() + " . " +
  Properties.Settings.Default.etIP_SubnetThree.ToString();*/

    //nudFirstIP.Value = ipNew[0] = ipCurrent[0] = Properties.Settings.Default.etIP_SubnetOne;
    //nudSecndIP.Value = ipNew[1] = ipCurrent[1] = Properties.Settings.Default.etIP_SubnetTwo;
    //nudThirdIP.Value = ipNew[2] = ipCurrent[2] = Properties.Settings.Default.etIP_SubnetThree;

    ipNew[0] = ipCurrent[0] = property_setUDP_IP1;
    ipNew[1] = ipCurrent[1] = property_setUDP_IP2;
    ipNew[2] = ipCurrent[2] = property_setUDP_IP3;


    ScanNetwork();
}

void FormLoop::timer1_Tick()
{
    if (!scanReply.isNewData)
    {
        ipAutoSet[0] = 99;
        ipAutoSet[1] = 99;
        ipAutoSet[2] = 99;
        //btnAutoSet.Enabled = false;
    }
    else
    {
        //btnAutoSet.Enabled = true;
    }

    if (scanReply.isNewSteer)
    {
        //lblSteerIP.Text = mf.scanReply.steerIP;
        scanReply.isNewSteer = false;
        //lblNewSubnet.Text = mf.scanReply.subnetStr;
    }

    if (scanReply.isNewMachine)
    {
        //lblMachineIP.Text = mf                    }
        scanReply.isNewMachine = false;
        //lblNewSubnet.Text = mf.scanReply.subnetStr;
    }

    if (scanReply.isNewIMU)
    {
        //lblIMU_IP.Text = mf.scanReply.IMU_IP;
        scanReply.isNewIMU = false;
        //lblNewSubnet.Text = mf.scanReply.subnetStr;
    }

    if (scanReply.isNewGPS)
    {
        //lblGPSIP.Text = mf.scanReply.GPS_IP;
        scanReply.isNewGPS = false;
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

void FormLoop::ScanNetwork()
{

    //tboxNets.Text = "";

    //lblSteerIP.Text = lblMachineIP.Text = lblGPSIP.Text = lblIMU_IP.Text = lblNewSubnet.Text = "";
    scanReply.isNewData = false;

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
                            //scanSocket->setSocketOption(QAbstractSocket::ReuseAddressHint, 1);
                            //this sets up the connection, and checks if it worked
                            /*if (scanSocket->bind(ethModulesSet.address, ethModulesSet.portToListen, QUdpSocket::ReuseAddressHint)){
                                qDebug() << "ScanSocket Sent out on address " << ethModulesSet.address << ":" << ethModulesSet.portToSend;
                                scanSocket->writeDatagram(scanModules, QHostAddress(ethModulesSet.address), ethModulesSet.portToSend);
                            }*/
                            QHostAddress address(info.ip().toString());

                            if (scanSocket->bind(address, ethModulesSet.portToListen, QUdpSocket::ReuseAddressHint)){
                                qDebug() << "ScanSocket bound to address " << address << ":" << ethModulesSet.portToSend;
                                scanSocket->writeDatagram(scanModules, ethModulesSet.address, ethModulesSet.portToSend);
                                qDebug() << "ScanSocket wrote to address " << ethModulesSet.address << ":" << ethModulesSet.portToSend;
                            }

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

void FormLoop::btnSendSubnet_Click()
{

    /*sendIPToModules[7] = ipNew[0];
    sendIPToModules[8] = ipNew[1];
    sendIPToModules[9] = ipNew[2];*/

    sendIPToModules[7] = property_setUDP_IP1;
    sendIPToModules[8] = property_setUDP_IP2;
    sendIPToModules[9] = property_setUDP_IP3;

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

                            scanSocket->bind(info.ip(), 9999);
                            scanSocket->writeDatagram(sendIPToModules, ethModulesSet.address, ethModulesSet.portToSend);
                            qDebug() << "IP Subnet " << ipNew[0] << "." << ipNew[1] << "." << ipNew[2] << " sent!";
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

    /*property_setUDP_IP1 = ipCurrent[0] = ipNew[0];
    property_setUDP_IP2 = ipCurrent[1] = ipNew[1];
    property_setUDP_IP3 = ipCurrent[2] = ipNew[2];*/

    ipNew[0] = ipCurrent[0] = property_setUDP_IP1;
    ipNew[1] = ipCurrent[1] = property_setUDP_IP2;
    ipNew[2] = ipCurrent[2] = property_setUDP_IP3;


    //Properties.Settings.Default.Save();
    //does the lower replace the upper? Remove if it does
    settings->sync(); //do we still need this with the new settings class?

    // update ethUDP from settings
    int ip1 = property_setUDP_IP1;
    int ip2 = property_setUDP_IP2;
    int ip3 = property_setUDP_IP3;
    int ip4 = 255; //broadcast

    quint32 ipAddress = (ip1 << 24) | (ip2 << 16) | (ip3 << 8) | ip4;

    ethUDP.address.setAddress(ipAddress);

    /*lblNetworkHelp.Text =
		Properties.Settings.Default.etIP_SubnetOne.ToString() + " . " +
		Properties.Settings.Default.etIP_SubnetTwo.ToString() + " . " +
        Properties.Settings.Default.etIP_SubnetThree.ToString();*/
}

//pboxSendSteer.Visible = false;
//btnSerialCancel.Image = Properties.Resources.back_button;
//}

void FormLoop::btnAutoSet_Click()
{

    //nudFirstIP.Value = mf.scanReply.subnet[0];
    //nudSecndIP.Value = mf.scanReply.subnet[1];
    //nudThirdIP.Value = mf.scanReply.subnet[2];
    ipNew[0] = scanReply.subnet[0];
    ipNew[1] = scanReply.subnet[1];
    ipNew[2] = scanReply.subnet[2];
    //btnSerialCancel.Image = Properties.Resources.Cancel64;
    //pboxSendSteer.Visible = true;
}
/*This is all UI. Don't know how much we'll reuse
void FormLoop::nudFirstIP_Click()
{
    ipNew[0] = (byte)nudFirstIP.Value;
    ipNew[1] = (byte)nudSecndIP.Value;
    ipNew[2] = (byte)nudThirdIP.Value;
    btnSendSubnet.Enabled = true;
	pboxSendSteer.Visible = true;
    btnSerialCancel.Image = Properties.Resources.Cancel64;
}*/
