using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Net;

namespace Tanky
{
    public class UDPBroadcaster
    {
        private const int m_BroadCastPort = 8000;
        private Socket m_Socket;
        private string m_BroadCastString;

        public UDPBroadcaster()
        {
            // Set up the socket
            m_Socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp); 
            m_Socket.SetSocketOption(SocketOptionLevel.Socket, SocketOptionName.Broadcast, 1);

            // Build the servers broadcast message
            m_BroadCastString = Environment.MachineName;
        }

        public void BroadCast()
        {
            // Use broad cast address
            IPEndPoint addr = new IPEndPoint(IPAddress.Broadcast, m_BroadCastPort);
            
            // Broadcast the data
            m_Socket.SendTo(System.Text.ASCIIEncoding.ASCII.GetBytes(m_BroadCastString), addr);

            //Console.WriteLine("Broadcasting server presense to the local network");
        }
    }

    public class UDPBroadCastCheck
    {
        private const int m_BroadCastPort = 8000;
        private byte[] m_SocketBuffer;
        private EndPoint m_BroadCastEndPoint;
        private Socket m_Socket;

        public UDPBroadCastCheck()
        {
            // Create the socket
            m_Socket = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
            
            // Set up the broad cast end point
            m_BroadCastEndPoint = new IPEndPoint(IPAddress.Broadcast, m_BroadCastPort);

            // Bind the socket to the local computer
            IPEndPoint localhost = new IPEndPoint(IPAddress.Any, m_BroadCastPort);
            m_Socket.Bind(localhost);

            // Create the buffer to read the server message
            m_SocketBuffer = new byte[256];

            // Start an async read of the data
            m_Socket.BeginReceiveFrom(m_SocketBuffer, 0, m_SocketBuffer.Length, SocketFlags.None, ref m_BroadCastEndPoint, OnUDPBroadCast, this);
        }

        private void OnUDPBroadCast( IAsyncResult aAsyncResult )
        {
            // Get the instance
            UDPBroadCastCheck instance = (UDPBroadCastCheck)aAsyncResult.AsyncState;

            // Finish the async event
            int bytesRead = instance.m_Socket.EndReceiveFrom(aAsyncResult, ref instance.m_BroadCastEndPoint);

            // Process the buffer we read
            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
            string server = enc.GetString(instance.m_SocketBuffer, 0, bytesRead);
            string ip = instance.m_BroadCastEndPoint.ToString();
            string[] parts = ip.Split(':');

            g.GameRef.OnFoundServer( server, parts[0] );

            // Reset the end point
            instance.m_BroadCastEndPoint = new IPEndPoint(IPAddress.Any, m_BroadCastPort);

            // Start an async read of the data
            m_Socket.BeginReceiveFrom(m_SocketBuffer, 0, m_SocketBuffer.Length, SocketFlags.None, ref m_BroadCastEndPoint, OnUDPBroadCast, this);
        }
    }
}
