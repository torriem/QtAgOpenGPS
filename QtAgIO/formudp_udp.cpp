#include "formudp.h"


void FormUDp_Load(FormLoop &mf)
{
    mf.ipAutoSet[0] = 99;
    mf.ipAutoSet[1] = 99;
    mf.ipAutoSet[2] = 99;

    //lblHostname.Text = Dns.GetHostName(); // Retrieve the Name of HOST

    /*lblNetworkHelp.Text =
  Properties.Settings.Default.etIP_SubnetOne.ToString() + " . " +
  Properties.Settings.Default.etIP_SubnetTwo.ToString() + " . " +
  Properties.Settings.Default.etIP_SubnetThree.ToString();*/

    nudFirstIP.Value = ipNew[0] = ipCurrent[0] = Properties.Settings.Default.etIP_SubnetOne;
    nudSecndIP.Value = ipNew[1] = ipCurrent[1] = Properties.Settings.Default.etIP_SubnetTwo;
    nudThirdIP.Value = ipNew[2] = ipCurrent[2] = Properties.Settings.Default.etIP_SubnetThree;

    ScanNetwork();
}
int tickCounter = 0;

void FormUDP::timer1_Tick(FormLoop &mf)
{
    if (!mf.scanReply.isNewData)
    {
        mf.ipAutoSet[0] = 99;
        mf.ipAutoSet[1] = 99;
        mf.ipAutoSet[2] = 99;
        //btnAutoSet.Enabled = false;
    }
    else
    {
        //btnAutoSet.Enabled = true;
    }

    if (mf.scanReply.isNewSteer)
    {
        //lblSteerIP.Text = mf.scanReply.steerIP;
        mf.scanReply.isNewSteer = false;
        //lblNewSubnet.Text = mf.scanReply.subnetStr;
    }

    if (mf.scanReply.isNewMachine)
    {
        //lblMachineIP.Text = mf                    }
.scanReply.machineIP;
        mf.scanReply.isNewMachine = false;
        //lblNewSubnet.Text = mf.scanReply.subnetStr;
    }

    if (mf.scanReply.isNewIMU)
    {
        //lblIMU_IP.Text = mf.scanReply.IMU_IP;
        mf.scanReply.isNewIMU = false;
        //lblNewSubnet.Text = mf.scanReply.subnetStr;
    }

    if (mf.scanReply.isNewGPS)
    {
        //lblGPSIP.Text = mf.scanReply.GPS_IP;
        mf.scanReply.isNewGPS = false;
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

void FormUDP::ScanNetwork(FormLoop &mf)
{

    //tboxNets.Text = "";

    //lblSteerIP.Text = lblMachineIP.Text = lblGPSIP.Text = lblIMU_IP.Text = lblNewSubnet.Text = "";
    mf.scanReply.isNewData = false;

    bool isSubnetMatchCard = false;

    QByteArray scanModules = { 0x80, 0x81, 0x7F, 202, 3, 202, 202, 5, 0x47 };

    //Send out 255x4 to each installed network interface
    foreach (var nic in NetworkInterface.GetAllNetworkInterfaces())
    {
        if (nic.Supports(NetworkInterfaceComponent.IPv4))
        {
            foreach (var info in nic.GetIPProperties().UnicastAddresses)
            {
                // Only InterNetwork and not loopback which have a subnetmask
                if (info.Address.AddressFamily == AddressFamily.InterNetwork && !IPAddress.IsLoopback(info.Address))
                {
                    QUdpSocket scanSocket;
                    //Socket scanSocket;

                    try
                    {
                        //create list of interface properties
                        if ((cboxUp.Checked && nic.OperationalStatus == OperationalStatus.Up) || !cboxUp.Checked)
                        {
                            var properties = nic.GetIPStatistics();
                            //tboxNets.Text +=
                            //	info.Address + "  - " + nic.OperationalStatus + "\r\n";

                            //tboxNets.Text += nic.Name.ToString() + "\r\n";
                            /*tboxNets.Text += "Sent: " + (properties.NonUnicastPacketsSent
									+ properties.UnicastPacketsSent).ToString()
								+ "   Recd: " + (properties.NonUnicastPacketsReceived
                                        + properties.UnicastPacketsReceived).ToString() + "\r\n\r\n";*/
                        }

                        if (nic.OperationalStatus == OperationalStatus.Up
                            && info.IPv4Mask != null)
                        {
                            QByteArray data = info.Address.GetAddressBytes();
                            if (data[0] == ipCurrent[0] && data[1] == ipCurrent[1] && data[2] == ipCurrent[2])
                            {
                                isSubnetMatchCard = true;
                            }

                            //send scan reply out each network interface
                            /*Did I set the options all right? David
                             * scanSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, true);
                            scanSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
                            scanSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.DontRoute, true);*/

                            scanSocket.setSocketOption(QAbstractSocket::MulticastLoopbackOption, true);
                            scanSocket.setSocketOption(QAbstractSocket::ShareAddress, true);
                            scanSocket.setSocketOption(QAbstractSocket::DontRouteOption, true);
                            //this sets up the connection, and checks if it worked
                            if (scanSocket.bind(QHostAddress(mf.epModuleSet), 9999))
                                scanSocket.SendTo(scanModules, 0, scanModules.Length, SocketFlags.None, mf.epModuleSet);
                            else
                                qDebug() << "ScanSocket error: " << scanSocket.errorString();



                            scanSocket.close();
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.Write("nic Loop = ");
                        Console.WriteLine(ex.ToString());
                    }
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

void FormUDP::btnSendSubnet_Click(FormLoop &mf)
{

    sendIPToModules[7] = ipNew[0];
    sendIPToModules[8] = ipNew[1];
    sendIPToModules[9] = ipNew[2];

    //loop thru all interfaces
    foreach (var nic in NetworkInterface.GetAllNetworkInterfaces())
    {
        if (nic.Supports(NetworkInterfaceComponent.IPv4) && nic.OperationalStatus == OperationalStatus.Up)
        {
            foreach (var info in nic.GetIPProperties().UnicastAddresses)
            {
                // Only InterNetwork and not loopback which have a subnetmask
                if (info.Address.AddressFamily == AddressFamily.InterNetwork &&
                    !IPAddress.IsLoopback(info.Address) &&
                    info.IPv4Mask != null)
                {
                    //Socket scanSocket;
                    QUdpSocket scanSocket;
                    try
                    {
                        if (nic.OperationalStatus == OperationalStatus.Up
                            && info.IPv4Mask != null)
                        {
                            scanSocket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
                            scanSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, true);
                            scanSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.ReuseAddress, true);
                            scanSocket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.DontRoute, true);

                            try
                            {
                                scanSocket.Bind(new IPEndPoint(info.Address, 9999));
                                scanSocket.SendTo(sendIPToModules, 0, sendIPToModules.Length, SocketFlags.None, mf.epModuleSet);
                            }
                            catch (Exception ex)
                            {
                                Console.Write("Bind Error = ");
                                Console.WriteLine(ex.ToString());
                            }

                            scanSocket.Dispose();
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.Write("nic Loop = ");
                        Console.WriteLine(ex.ToString());
                    }
                }
            }
        }
    }

    Properties.Settings.Default.etIP_SubnetOne = ipCurrent[0] = ipNew[0];
    Properties.Settings.Default.etIP_SubnetTwo = ipCurrent[1] = ipNew[1];
    Properties.Settings.Default.etIP_SubnetThree = ipCurrent[2] = ipNew[2];

    Properties.Settings.Default.Save();

    mf.epModule = new IPEndPoint(IPAddress.Parse(
                                     Properties.Settings.Default.etIP_SubnetOne.ToString() + "." +
                                     Properties.Settings.Default.etIP_SubnetTwo.ToString() + "." +
                                     Properties.Settings.Default.etIP_SubnetThree.ToString() + ".255"), 8888);

    /*lblNetworkHelp.Text =
		Properties.Settings.Default.etIP_SubnetOne.ToString() + " . " +
		Properties.Settings.Default.etIP_SubnetTwo.ToString() + " . " +
        Properties.Settings.Default.etIP_SubnetThree.ToString();*/
}

//pboxSendSteer.Visible = false;
//btnSerialCancel.Image = Properties.Resources.back_button;
}

void FormUDP::btnAutoSet_Click(FormLoop &mf)
{

    nudFirstIP.Value = mf.scanReply.subnet[0];
    nudSecndIP.Value = mf.scanReply.subnet[1];
    nudThirdIP.Value = mf.scanReply.subnet[2];
    ipNew[0] = mf.scanReply.subnet[0];
    ipNew[1] = mf.scanReply.subnet[1];
    ipNew[2] = mf.scanReply.subnet[2];
    //btnSerialCancel.Image = Properties.Resources.Cancel64;
    //pboxSendSteer.Visible = true;
}
void FormUDP::nudFirstIP_Click()
{
    ipNew[0] = (byte)nudFirstIP.Value;
    ipNew[1] = (byte)nudSecndIP.Value;
    ipNew[2] = (byte)nudThirdIP.Value;
    /*btnSendSubnet.Enabled = true;
	pboxSendSteer.Visible = true;
    btnSerialCancel.Image = Properties.Resources.Cancel64;*/
}
