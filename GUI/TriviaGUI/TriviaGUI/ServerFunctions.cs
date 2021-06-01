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

        public static bool GetCurrentPlayersInRoom(TcpClient serverConnection, int roomId, out string[] completedData)
        {
            completedData = new string[1000];
            Dictionary<string, object> RoomDitalis = new Dictionary<string, object>();
            RoomDitalis.Add("roomID", roomId.ToString());
            string json_parsed = JsonConvert.SerializeObject(RoomDitalis);
            byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed);
            byte[] data_encoded = ServerFunctions.getCompleteMsg(5, json_byted);

            serverConnection.GetStream().Write(data_encoded, 0, 1000);

            while (serverConnection.Available == 0) ;
            //System.Threading.Thread.Sleep(100);
            byte[] serverOutput = ServerFunctions.ReadServerMessage(serverConnection);
            Newtonsoft.Json.Linq.JObject jsonReturned = ServerFunctions.diserallizeResponse(serverOutput);
            try
            {
                if (jsonReturned["players"].ToString() != "")
                {
                    string[] seperators = { ", " };
                    string[] players = jsonReturned["players"].ToString().Split(seperators, System.StringSplitOptions.RemoveEmptyEntries);
                    completedData = players;
                    return true;
                }
            }
            catch (Exception e)
            { }
            return false;
        }

        /* retrieving a specific room data*/
        public static bool GetRoomDataString(TcpClient serverConnection, string roomName, int room_id, out string completedData)
        {
            completedData = "";
            Dictionary<string, object> RoomDitalis = new Dictionary<string, object>();
            RoomDitalis.Add("roomID", room_id.ToString()); // inserting room id to json

            string json_parsed = JsonConvert.SerializeObject(RoomDitalis); // serializing data
            byte[] json_byted = System.Text.Encoding.ASCII.GetBytes(json_parsed); // encoding json
            byte[] data_encoded = ServerFunctions.getCompleteMsg(9, json_byted); // adding size and code for protocal

            serverConnection.GetStream().Write(data_encoded, 0, 1000);

            while (serverConnection.Available == 0) ; // wait until a new message arrived from the server
            byte[] serverOutput = ServerFunctions.ReadServerMessage(serverConnection);
            Newtonsoft.Json.Linq.JObject jsonReturned = ServerFunctions.diserallizeResponse(serverOutput); // diserallizing json from server
            try
            {
                string[] players;
                if (jsonReturned["status"].ToString() == "1" && jsonReturned["isActive"].ToString() == "1") // checks if no error has occurred and if the server is active
                {
                    completedData += room_id.ToString();

                    if (ServerFunctions.GetCurrentPlayersInRoom(serverConnection, room_id, out players) == true) // getting the current players in room and checking if the number of max player is valid
                    {
                        // writing the room data to a string
                        completedData += "," + roomName + "," + players.Length.ToString() + "," + jsonReturned["maxPlayers"].ToString() + "," +
                          jsonReturned["questionsNumber"].ToString() + "," + jsonReturned["timePerQuestion"].ToString();
                      /*  completedData += roomName + "         ";
                        completedData += players.Length.ToString() + " / " + jsonReturned["maxPlayers"].ToString() + "                                        ";
                        completedData += jsonReturned["questionsNumber"].ToString() + "                                    ";
                        completedData += jsonReturned["timePerQuestion"].ToString();*/
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }

            }
            catch (Exception e)
            { }
            return false;
        }

    }
}
