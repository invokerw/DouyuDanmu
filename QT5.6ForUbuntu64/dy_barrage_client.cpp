/*
 *  COPYRIGHT NOTICE
 *  Copyright (c) 2015, Douyu Network Technology Co., Ltd. in Wuhan
 *  All rights reserved.
 *
 *  @version : 1.0
 *  @author : mxl
 *  @E-mail：xiaolongicx@gmail.com
 *  @date : 2015/5/19
 *
 *  Revision Notes :
*/

#include "data_def.h"
#include "dy_barrage_client.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>

#ifdef _MSC_VER
#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#endif

using namespace std;

#define MAX_DATA_SIZE 40960   //maximum length(bytes) of each reception


dy_barrage_client::dy_barrage_client()
{

}
dy_barrage_client::~dy_barrage_client()
{

}

int dy_barrage_client::connect_dy_server(const char *host_p, int port)
{
	struct hostent *host;

#ifdef _MSC_VER
	unsigned long lgIP = inet_addr(host_p);
	//输入的IP字符串
	if(lgIP != INADDR_NONE)
	{
		WSACleanup();
		return -1;
	}
	WSADATA wsaData;
	WSAStartup(MAKEWORD(1,1),&wsaData);
#endif

	host = gethostbyname(host_p);
	if(host == NULL)
	{

        //cout << "gethostbyname err, host is" << host_p << endl;
		return -1;
	}

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1)
    {
       // cout << "create socket failed!" << endl;
        return -1;
    }

	struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
    //bzero(&(serv_addr.sin_zero), 8);
	memset(serv_addr.sin_zero, 0, 8);

    int con_ret = connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));
    if(con_ret == -1)
    {
        cout << "connect server " << host_p << ":" << port << " err!" << endl;
        return -1;
    }
    else
    {
        cout << "connect server " << host_p << ":" << port << " ok" << endl;
    }
    return 0;
}

int dy_barrage_client::login_dy_room()
{
    int ret = 0;
    dy_login_req login_req;
    string data = login_req.transform_to_dy_string();

    //cout << data << endl;

    ret = send(sock_fd, data.data(), data.size(), 0);
    if(ret == -1)
    {
        cout << "send login request err: " << ret << endl;
    }
    else
    {
        cout << "send login request ok" << endl;
    }

    // receive login response
    QString s,n;
    get_dy_server_msg(s,n);

    return 0;
}

int dy_barrage_client::join_dy_room_group(int room_id, int group_id)
{
    int ret = 0;

    dy_join_group_req join_req(room_id, group_id);

    string data = join_req.transform_to_dy_string();

    ret = send(sock_fd, data.data(), data.size(), 0);
    if(ret == -1)
    {
        cout << "send join group request err: " << ret << endl;
    }
    else
    {
        cout << "send join group ok" << endl;
    }

    return 0;
}

void dy_barrage_client::get_dy_server_msg(QString &name,QString &say)
{
    name = "";
    say  = "";

    int packet_length = 0;
    // get the packet length 4 bytes
    int recv_bytes = recv(sock_fd, reinterpret_cast<char *>(&packet_length), 4, 0);
    if (recv_bytes == -1)
    {
        cout << "receive server message err!" << endl;
        return;
    }

    if (packet_length <= 8)
    {
        return;
    }

    // get the packet body packet_length bytes
    char buf[MAX_DATA_SIZE] = {0};
	//wchar_t
    recv_bytes = recv(sock_fd, buf, packet_length, 0);
    if(recv_bytes == -1)
    {
        cout << "receive server message err!" << endl;
        return;
    }
    else
    {
        //you should better paste the packet as douyu packet header length, if not something unpredictable may happen
		//cout<<"pack length:"<<recv_bytes<<endl;
		
        //cout<<"hear"<<endl;

		string pack_data(buf, recv_bytes);
		
		//cout<<pack_data<<endl;
        int msg_type = get_dy_msg_type(pack_data);

        switch(msg_type)
        {
            case MSG_TYPE_LOGIN_RESPONSE:
                on_login_response(pack_data);
                break;
            case MSG_TYPE_BARRAGE:
                {
                //on_barrage(pack_data);
                dy_barrage_res barr_res;
                //cout<<data<<endl;
                barr_res.parse_from_dy_string(pack_data);
                //QString::fromStdString(str)
                name = QString::fromStdString(barr_res.sender_name);
                say  = QString::fromStdString(barr_res.content);
                // cout<<barr_res.sender_name<<":"<<barr_res.content<<endl;
                }
                break;
            // deal other server message as you need here
            default:
                break;
        }
    }
}

void dy_barrage_client::keep_alive()
{
#ifdef _MSC_VER
	dy_keep_alive_req keepl_req(GetTickCount());
#else
	dy_keep_alive_req keepl_req(time(NULL));
#endif


    string data = keepl_req.transform_to_dy_string();

    int ret = send(sock_fd, data.data(), data.size(), 0);
    if(ret == -1)
    {
        cout << "send keep alive request err: " << ret << endl;
    }


}
string UTF8ToANSI(const char *szU8)
{
	//预转换，得到所需空间的大小
   // int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), NULL, 0);
    //分配空间要给'\0'留个空间，MultiByteToWideChar不会给'\0'空间
  //  wchar_t* wszString = new wchar_t[wcsLen + 1];
    //转换
  //  ::MultiByteToWideChar(CP_UTF8, NULL, szU8, strlen(szU8), wszString, wcsLen);
    //最后加上'\0'
   // wszString[wcsLen] = '\0';

	string szDst;
	//DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,wszString,-1,NULL,0,NULL,FALSE);
	
	//char *psText;
	//psText = new char[dwNum];
	//WideCharToMultiByte (CP_OEMCP,NULL,wszString,-1,psText,dwNum,NULL,FALSE);
	//szDst = psText;
	//cout<<szDst<<endl;
	// 删除临时变量
//	delete []psText;
	return szDst;
}
void dy_barrage_client::on_barrage(string data)
{

    dy_barrage_res barr_res;
	//cout<<data<<endl;
    barr_res.parse_from_dy_string(data);
    //QString::fromStdString(str)
    //add_line_to_tableview(QString::fromStdString(barr_res.sender_name),QString::fromStdString(barr_res.content));
    cout<<barr_res.sender_name<<":"<<barr_res.content<<endl;

}

void dy_barrage_client::on_login_response(string data)
{

    dy_login_res login_res;
    login_res.parse_from_dy_string(data);

    if (0 == login_res.ret)
    {
        cout << "login successfully" << endl;
    }
    else
    {
        cout << "login err: " << login_res.ret << endl;
    }
}
//void dy_barrage_client::add_line_to_tableview(QString name,QString say)
//{
//    int x = m_qsimp->rowCount();
//    m_qsimp->setItem(x-1, 0, new QStandardItem(name));
//    m_qsimp->setItem(x-1, 1, new QStandardItem(say));
//    m_qsimp->item(1, 0)->setTextAlignment(Qt::AlignCenter);

//}
