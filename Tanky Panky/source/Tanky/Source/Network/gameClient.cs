using System;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Collections.Generic;
using System.IO.Compression;

namespace Tanky
{
    public enum PacketFams
    {
        EPlayerManagement  = 0x01,   // Player join/leave/name update
        ESessionManagement = 0x02,   // Setup/close down connections
        EMovementData      = 0x03,
    }

    public enum PacketSubTypes
    {
        // EPlayerManagement sub types
        EPlayerConnected      = 0x01,    // A player has connected
        EPlayerMessage        ,    // A player has typed a message
        EPlayerDisconnected   ,    // A player has disconnected
        EPlayerPingRequest    ,    // A players ping request from the server
        EPlayerPingResponse   ,    // A players ping response from the client
        EPlayerPingUpdate     ,    // A players ping data, contains "PlayerName:Ping"
        EPlayerSettingsUpdate ,    // A player is requesting to set team, name and model
        EPlayerNameUpdate     ,    // From the server, with the old player name and the new player name
        EPlayerTeamUpdate     ,    // From the server, a player has updated what team its on
        EPlayerModelUpdate    ,    // From the server, a player had updated what player model it has
        EPlayerSpawnData      ,    // From the server telling a player where to spawn
        EPlayerReSpawn        ,    // From the server, telling a client it has died and needs to respawn
        EPlayerAnnouce        ,    // From the server to the client, an accouncement message such as player being killed

        EPlayerScoreKillsUpdate,    // From the server containing only the kills and score of the player

        // ESessionManagement sub types
        ESessionIsServerFull    ,    // Check if the server is full
        ESessionServerFull      ,    // Server response, not full
        ESessionServerNotFull   ,    // Server response, server full
        ESessionForceNewName    ,    // Player name was taken on the server so the server has given us this name instead
        ESessionStartGame       ,    // From the server telling us to start the game
        ESeesionEndGame         ,    // From the server to clients telling them that the game is over
        ESessionTimerUpdate     ,    // From the server with an updated game time

        ESessionPlayerDead      ,    // A client telling us that they have died

        // EMovementData sub types
        EMovementUpdatePlayer           ,   // New position of player entity in the world
        EMovementUpdateProp             ,   // New postion of prop in the world
        EMovementRequestFireProjectile  ,   // Requesting to fire a shot
        EMovementSpawnProjectile        ,   // A projectile needs to be spawned
        EMovementProjectileHit          ,   // Information about a projectile that has hit a player and done damage 

    }

    public struct PacketHeader
    {
        public byte m_Magic;
        public UInt16 m_Length;
        public UInt16 m_DecompressedLength;
        public PacketFams m_Family;
        public PacketSubTypes m_Subtype;
    }

    public class gameClient
    {
        // Info about the player connected with this socket
        public Player m_PlayerInfo;

        // Socket from server to remote player/host
        public Socket m_Socket;

        // Data buffer to read socket data into
        public byte[] m_SocketBuffer;
        public int m_SocketBufferLen;

        private byte[] m_SocketBufferRemainder;
        private int m_SocketBufferRemainderLen;

        private const int m_SocketBufferSize = 1024*20;

        public bool m_PingReponsePending;
        public float m_PingMillisecondsElapsed;

        public gameClient(string aHost)
        {
            try
            {
                m_Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                IPHostEntry dns = Dns.GetHostEntry(aHost);

                IPEndPoint ipEnd = new IPEndPoint(dns.AddressList[0], gameServer.m_ListenPort);

                AsyncCallback connectCallBack = new AsyncCallback(OnConnectComplete);

                //Console.WriteLine("Starting async connect to " + dns.AddressList[0].ToString());

                m_SocketBuffer = new byte[m_SocketBufferSize];

                m_Socket.Blocking = false;
                m_Socket.BeginConnect(ipEnd, connectCallBack, this);
            }
            catch
            {
                g.GameRef.OnClientConnectError( "Can't resolve host name" );
            }
        }

        public void OnConnectComplete( IAsyncResult aAsyncEvent )
        {
            try
            {
                // Get this instance
                gameClient instance = (gameClient)aAsyncEvent.AsyncState;

                // Finish up the connection
                instance.m_Socket.EndConnect(aAsyncEvent);

                //Console.WriteLine("CLIENT: Server Full?");
                SendMessage(PacketFams.ESessionManagement, PacketSubTypes.ESessionIsServerFull, "");

                // Start reading data
                ReadNewData();
            }
            catch
            {
                g.GameRef.OnClientConnectError("Failed to connect to the server!");
            }
        }

        public gameClient( ref TcpListener aTcpServer )
        {
            m_SocketBuffer = new byte[m_SocketBufferSize];

            // Accept the connection from the listening server socket
            m_Socket = aTcpServer.AcceptSocket();
            m_Socket.Blocking = false;

            //Console.WriteLine("new client connected from " + m_Socket.RemoteEndPoint.ToString());
        }

        private void SendData( byte[] aData, int aDataLen )
        {
            // TODO: If already sending then buffer the data?
            if (m_Socket.Connected)
            {
                try
                {
                    ////Console.WriteLine("Sending data");
                    AsyncCallback newDataCallBack = new AsyncCallback(OnDataSent);
                    m_Socket.BeginSend(aData, 0, aDataLen, SocketFlags.None, OnDataSent, this);
                }
                catch (Exception)
                {
                    //Console.WriteLine("Error sending data!");
                    if (!g.is_server)
                    {
                        g.GameRef.OnClientConnectError("Connection to server lost");
                    }
                }
            }
            else if (!g.is_server)
            {
                g.GameRef.OnClientConnectError("Connection to server lost (send error)");
            }
            else
            {
                if (!g.is_server)
                {
                    g.GameRef.OnClientConnectError("No connection!");
                }
            }
        }

        private void OnDataSent(IAsyncResult aAsyncEvent)
        {
            gameClient instance = (gameClient)aAsyncEvent.AsyncState;
            try
            {
                instance.m_Socket.EndSend(aAsyncEvent);
                ////Console.WriteLine("Send complete!");
            }
            catch (Exception)
            {
                if (!g.is_server)
                {
                    g.GameRef.OnClientConnectError("Connection to server lost (write error)");
                }
            }
        }

        public void ReadNewData()
        {
            if (m_Socket.Connected)
            {
                ////Console.WriteLine("Start async read");

                AsyncCallback newDataCallBack = new AsyncCallback(OnNewData);
                try
                {
                    m_Socket.BeginReceive(m_SocketBuffer, 0, m_SocketBufferSize, SocketFlags.None, newDataCallBack, this);

                }
                catch (Exception)
                {
                    //Console.WriteLine("Start async read error!");
                    if (!g.is_server)
                    {
                        g.GameRef.OnClientConnectError("Connection to server lost");
                    }
                }
            }
            else
            {
                if (!g.is_server)
                {
                    g.GameRef.OnClientConnectError("Connection to server lost");
                }
            }
            
        }

        public void OnNewData(IAsyncResult aAsyncEvent)
        {
            // Get this instance
            gameClient instance = (gameClient)aAsyncEvent.AsyncState;
            try
            {
                // Finish getting data
                m_SocketBufferLen = instance.m_Socket.EndReceive(aAsyncEvent);
            }
            catch (Exception)
            {
                if (!g.is_server)
                {
                    g.GameRef.OnClientConnectError("Connection to server lost (read error)");
                }
            }

            if (m_SocketBufferLen != 0)
            {
                // Parse the data that we have
                ParsePacketBuffer(m_SocketBuffer,m_SocketBufferLen);

                // Get the next set of data
                ReadNewData();
            }
            else
            {
                //Console.WriteLine("Got zero bytes, socket closed!");

                if (!g.is_server)
                {
                    g.GameRef.OnClientConnectError("Connection to server lost");
                }
            }

        }

        private void ParsePacketBuffer(byte[] aPacketData, int aPacketDataLen)
        {
            const int KPacketHeaderSize = 9; // 1 byte and 4 words = (4*2)+1

            // If there is data left over then add it on here!
            if (m_SocketBufferRemainderLen != 0)
            {
                ////Console.WriteLine("Data left over, concatanating it");
                aPacketDataLen = aPacketDataLen + m_SocketBufferRemainderLen;
  
                // Create a new buffer to hold the remainder and the new data
                byte[] concatBuf = new byte[aPacketDataLen];
                
                // Copy the reminder into it
                for ( int i=0; i<m_SocketBufferRemainderLen; i++ )
                {
                    concatBuf[i] = m_SocketBufferRemainder[i];
                }

                // Copy the new data into it
                for (int i = 0; i < (aPacketDataLen - m_SocketBufferRemainderLen); i++)
                {
                    concatBuf[i + m_SocketBufferRemainderLen] = aPacketData[i];
                }

                // Set aPacketData to the new buffer
                aPacketData = concatBuf;

                // Invalid the old buffer
                m_SocketBufferRemainderLen = 0;
            }

            // Is there enough data for a header?
            if (aPacketDataLen < KPacketHeaderSize)
            {
                // Not enough data for a header so we can't do anything!
                //Console.WriteLine("Need more data for this packet");
                
                // Not enough data so buffer it up
                int len = aPacketDataLen + m_SocketBufferRemainderLen;

                // Create a new buffer to hold the remainder and the new data
                byte[] concatBuf = new byte[len];

                // Copy the reminder into it
                for (int i = 0; i < m_SocketBufferRemainderLen; i++)
                {
                    concatBuf[i] = m_SocketBufferRemainder[i];
                }

                // Copy the new data into it
                for (int i = 0; i < aPacketDataLen; i++)
                {
                    concatBuf[i + m_SocketBufferRemainderLen] = aPacketData[i];
                }

                // Set m_SocketBufferRemainder to the new buffer
                m_SocketBufferRemainder = concatBuf;

                // Update the old buffer
                m_SocketBufferRemainderLen = len;
            }
            else
            {
                int offset = 0;
                System.IO.MemoryStream stm = new System.IO.MemoryStream(aPacketData, offset, aPacketDataLen - offset);
                System.IO.BinaryReader rdr = new System.IO.BinaryReader(stm);

                PacketHeader header;
                header.m_Magic = rdr.ReadByte();

                if (header.m_Magic != 0x2A)
                {
                    //Console.WriteLine("Courrpted packed, invalid header!");
                    if (!g.is_server)
                    {
                        g.GameRef.OnClientConnectError("Bad data");
                    }
                }

                header.m_Length = rdr.ReadUInt16();
                header.m_DecompressedLength = rdr.ReadUInt16();
                header.m_Family = (PacketFams)rdr.ReadUInt16();
                header.m_Subtype = (PacketSubTypes)rdr.ReadUInt16();

                // Subtract the size of the header
                int dataLen = aPacketDataLen - KPacketHeaderSize;

                // See if we have enough data for this packet
                if (dataLen == header.m_Length)
                {
                    // Exactly 1 packet so get the payload as a string for now
                    byte[] payload;
                    payload = rdr.ReadBytes(header.m_Length);
                    
                    // Handle this packet
                    OnNewPacket(header, payload);
                }
                else if (dataLen > header.m_Length)
                {
                    // 1 Packet and some more data, grab the payload of this packet
                    byte[] payload;
                    payload = rdr.ReadBytes(header.m_Length);

                    // Handle this packet
                    OnNewPacket(header, payload);

                    // Convert to string and show in console
                    //Console.WriteLine("More data, so processing whats left of the packet");

                    // Get how many more bytes of data are left
                    int pos = (int)rdr.BaseStream.Position;
                    int bytesLeft = Math.Abs(pos - aPacketDataLen);

                    // Extract them
                    byte[] remainder = rdr.ReadBytes(bytesLeft);

                    // Parse them
                    ParsePacketBuffer(remainder, bytesLeft);
                }
                else
                {
                    // Not enough data for this packet so store it until more data is read from the socket
                    //Console.WriteLine("Need more data for this packet");

                    int len = aPacketDataLen + m_SocketBufferRemainderLen;

                    // Create a new buffer to hold the remainder and the new data
                    byte[] concatBuf = new byte[len];

                    // Copy the reminder into it
                    for (int i = 0; i < m_SocketBufferRemainderLen; i++)
                    {
                        concatBuf[i] = m_SocketBufferRemainder[i];
                    }

                    // Copy the new data into it
                    for (int i = 0; i <aPacketDataLen; i++)
                    {
                        concatBuf[i + m_SocketBufferRemainderLen] = aPacketData[i];
                    }

                    // Set m_SocketBufferRemainder to the new buffer
                    m_SocketBufferRemainder = concatBuf;

                    // Update the old buffer
                    m_SocketBufferRemainderLen = len;
                }
            }
        }

        private void OnNewPacket( PacketHeader aHeader, byte[] aData )
        {
            // Decompress the packet
            byte[] dataCopy = aData;
            DecompressPacket(ref aHeader, ref aData);

            // Convert aData to a string for now
            System.Text.ASCIIEncoding enc = new System.Text.ASCIIEncoding();
            string temp = enc.GetString(aData, 0, aHeader.m_DecompressedLength);
            
            if (g.server != null)
            {
                // Give the packet to the server to deal with
                g.server.AddPacketToQueue(this, aHeader, aData);
            }

            // Let the game engine deal with the msg
            g.GameRef.AddPacketToQueue(this, aHeader, aData);
        }

        public void SendMessage(PacketFams aFam, PacketSubTypes aSubType, byte[] aData)
        {
            // Build the header
            PacketHeader header = new PacketHeader();
            header.m_Magic = 0x2A;
            header.m_Family = aFam;
            header.m_Subtype = aSubType;
            header.m_DecompressedLength = (ushort)aData.Length;
            //CompressPacket(ref header, ref aData);
            SendPrebuiltMessage(header, aData);
        }

        public void SendMessage(PacketFams aFam, PacketSubTypes aSubType, string aData )
        {
            // Build the header
            PacketHeader header = new PacketHeader();
            header.m_Magic = 0x2A;
            header.m_Family = aFam;
            header.m_Subtype = aSubType;
            header.m_DecompressedLength = (ushort)aData.Length;

            //byte[] data = new byte[aData.Length];
            //char[] buf = aData.ToCharArray();

            //System.Buffer.BlockCopy(buf, 0, data, 0, aData.Length);
            System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();
            byte[] data = encoding.GetBytes(aData);

           // System.IO.MemoryStream stm = new System.IO.MemoryStream(data, 0, buf.Length);
            //System.IO.BinaryWriter wtr = new System.IO.BinaryWriter(stm);
           // wtr.Write(buf, 0, buf.Length);

            //CompressPacket(ref header, ref data);
            SendPrebuiltMessage(header, data);

        }

        public void SendPrebuiltMessage(PacketHeader aHeader, byte[] aData)
        {
            // All data in here is NOT compressed, so we need to recompress it
            CompressPacket(ref aHeader, ref aData);

            const int KPacketHeaderSize = 9; // 1 byte and 3 words = (3*2)+1

            int bufferLen = aHeader.m_Length + KPacketHeaderSize;
            byte[] packet = new byte[bufferLen];

            System.IO.MemoryStream stm = new System.IO.MemoryStream(packet, 0, bufferLen);
            System.IO.BinaryWriter wtr = new System.IO.BinaryWriter(stm);

            // Copy aHeader into the packet
            wtr.Write(aHeader.m_Magic);
            wtr.Write(aHeader.m_Length);
            wtr.Write(aHeader.m_DecompressedLength);
            wtr.Write((UInt16)aHeader.m_Family);
            wtr.Write((UInt16)aHeader.m_Subtype);

            // Copy aData into the the packet
            wtr.Write(aData, 0, aData.Length);

            // Send the packet over the wire
            SendData(packet, bufferLen);
        }

        void CompressPacket( ref PacketHeader aHeader, ref byte[] aData )
        {
            if (aData.Length > 0)
            {
                byte[] buffer = aData;
                MemoryStream ms = new MemoryStream();
                using (GZipStream zip = new GZipStream(ms, CompressionMode.Compress, true))
                {
                    zip.Write(buffer, 0, buffer.Length);
                }

                ms.Position = 0;
                MemoryStream outStream = new MemoryStream();

                byte[] compressed = new byte[ms.Length];
                ms.Read(compressed, 0, compressed.Length);

                byte[] gzBuffer = new byte[compressed.Length];
                System.Buffer.BlockCopy(compressed, 0, gzBuffer, 0, compressed.Length);
                aData = new byte[gzBuffer.Length];
                aData = gzBuffer;
                aHeader.m_Length = (ushort)gzBuffer.Length;
            }

        }

        void DecompressPacket(ref PacketHeader aHeader, ref byte[] aData)
        {
            if (aData.Length > 0)
            {
                byte[] gzBuffer = aData;
                using (MemoryStream ms = new MemoryStream())
                {
                    ms.Write(gzBuffer, 0, gzBuffer.Length);

                    byte[] buffer = new byte[aHeader.m_DecompressedLength];

                    ms.Position = 0;
                    using (GZipStream zip = new GZipStream(ms, CompressionMode.Decompress))
                    {
                        zip.Read(buffer, 0, aHeader.m_DecompressedLength);
                        aData = new byte[buffer.Length];
                        aData = buffer;
                    }
                }
            }
        }
    }
}
