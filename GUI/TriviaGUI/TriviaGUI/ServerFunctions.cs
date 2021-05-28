using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using TriviaGUI;
using System.Net.Sockets;

namespace ServerFunctions
{
    public static class ServerFunctions
    {
        /* getting message and adding the necessary parts to the message so it can be compatible with the server */
        public static byte[] getCompleteMsg(byte msgCode, byte[] jsonBytes)
        {
            byte[] data_encoded = new byte[1024];
            int size = jsonBytes.Length;

            data_encoded[0] = msgCode; // inserting message code

            // inserting size to message
            data_encoded[4] = (byte)(size % 256);
            size /= 256;
            data_encoded[3] = (byte)(size % 256);
            size /= 256;
            data_encoded[2] = (byte)(size % 256);
            size /= 256;
            data_encoded[1] = (byte)(size % 256);
            size /= 256;

            // adding json to the complete message
            for (int i = 0; i < jsonBytes.Length; i++)
            {
                data_encoded[i + 5] = jsonBytes[i];
            }
            return data_encoded;
        }

        /* diserallizing the message from the server */
        public static Newtonsoft.Json.Linq.JObject diserallizeResponse(byte[] response)
        {
            byte[] dataDecoded = new byte[1024];
            int msgSize = 0;

            // copying response to dataDecode
            for (int i = 0; i < dataDecoded.Length; i++)
            {
                dataDecoded[i] = response[i];
            }

            msgSize = dataDecoded[4];
            msgSize += dataDecoded[3] * 256;
            msgSize += dataDecoded[2] * 256 * 256;
            msgSize += dataDecoded[1] * 256 * 256 * 256;

            for (int i = 0; i < dataDecoded.Length; i++)
            {
                dataDecoded[i] = 0;
            }
            for (int i = 0; i < msgSize; i++)
            {
                dataDecoded[i] = response[i + 5];
            }
            string jsonObjectInString = System.Text.Encoding.ASCII.GetString(dataDecoded);
            return (Newtonsoft.Json.Linq.JObject)JsonConvert.DeserializeObject(jsonObjectInString);
        }
        public static void ConnectingToServer()
        {
            if (TriviaGUI.App.Current.Properties["server"] == null)
            {
                while (true)
                {
                    try
                    {
                        App.Current.Properties["server"] = new TcpClient("127.0.0.1", 9999);
                        string t = "hello";
                        ((TcpClient)App.Current.Properties["server"]).GetStream().Write(System.Text.Encoding.ASCII.GetBytes(t), 0, System.Text.Encoding.ASCII.GetBytes(t).Length);
                        //bytes_written = 5;
                        byte[] b = new byte[10];
                        ((TcpClient)App.Current.Properties["server"]).GetStream().Read(b, 0, 5);
                        return;
                    }
                    catch (Exception e)
                    {

                    }
                }
            }
        }
        public static byte[] ReadServerMessage(TcpClient serverConnection)
        {
            byte[] serverOutput = new byte[1024];
            serverConnection.GetStream().Read(serverOutput, 0, serverOutput.Length);
            return serverOutput;
        }
    }
}
