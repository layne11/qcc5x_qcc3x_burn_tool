
// QCC30xx_toolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "QCC30xx_tool.h"
#include "QCC30xx_toolDlg.h"
#include "afxdialogex.h"
#include "testflash.h"
#include "TestEngine.h"
#include <windows.h>
#include "portChoice.h"
#include <string>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define KEY_READ_BUFFER_LEN 128

typedef struct user_config{
	UINT8 chip_type;
	UINT8 trans_type;
	UINT32 trans_port;
	bool check_earse;
	bool check_burn;
	bool check_set_addr_and_name;
	bool check_get_addr_and_name;
	bool check_addr_auto_add;
	UINT8 addr_auto_add_count;
	/*bool check_trim;
	INT8 trim_cap;
	INT8 trim_freq;*/
	char btaddr[15];
	char btname[MAX_PATH];
	char file_path[MAX_PATH];
}user_config_t;

typedef struct _bt_addr{
	uint16 nap;
	uint8 uap;
	uint32 lap;
}bt_addr_t;

static const char* const CFG_DB_512X = "qcc512x_qcc302x.sdb:QCC512X_CONFIG";
static const char* const CFG_DB_514X = "qcc514x_qcc304x.sdb:QCC514X_CONFIG";
static user_config_t g_user_config;
static bool init_done = FALSE;
static int port_chioce_index = -1;
char g_trans_port_arr[6][32];
// CQCC30xx_toolDlg 对话框
void InitConsoleWindows()
{
	AllocConsole();

#if _MSC_VER <= 1200 //这个是vc6.0
		    freopen("CONOUT$", "w+t", stdout);
#else //这个是vc2003以上
	FILE *stream;
	freopen_s(&stream, "CONOUT$", "wt", stdout);
#endif // _MSC_VER > 1000
}

int str2hexArray(char *src, uint8 * dst)
{
	int len,i,j;
	uint8 *tmp;
	len = strlen(src);
	if(len%2 || 14 != len)
		return -1;

	j = 0;
	tmp = (uint8 *)malloc(len);
	for(i=0; i<len; i++) {
		if ((src[i] >= '0') && (src[i] <= '9')) {
			tmp[j] = src[i] - '0';
		 }else if((src[i] >= 'a') && (src[i] <= 'z')) {
			tmp[j] = src[i] - 'a' + 10;
		} else if((src[i] >= 'A') && (src[i] <= 'Z')) {
			tmp[j] = src[i] - 'A' + 10;
		} else if((i==4 || i==7) && src[i] == '-') {
			j--;
		} else {
			free(tmp);
			return -1;
		}
		j++;
	}
	for(i=0;i<6;i++){
		dst[i] = tmp[i*2]*16 + tmp[i*2+1];
	}
	free(tmp);

	return 0;
}

int addr2str(char *src, char * dst,uint8 type)
{
	int i,j,n,k;
	char *p;
	memset(dst,'0',14);
	dst[4] = '-';
	dst[7] = '-';
	dst[14] = '\0';
	j=0;
	if(type) {
		for (i=0; '\0' != src[i]; i++)
		{
			if(src[i]=='0' && src[i+1]=='x')
				n=i+2;
			if(src[i]==',' || src[i]=='}'){
				k=i;
				if(j == 0){
					memcpy(&dst[6+n-k+8], &src[n], k-n);
				}else if(j == 1){
					memcpy(&dst[2+n-k+5],&src[n],k-n);
				}else if(j == 2){
					memcpy(&dst[4+n-k],&src[n],k-n);
				}
				n=0;
				k=0;
				j++;
			}
		}
	} else {
		memcpy(dst, src+17, 2);
		memcpy(dst+2, src+14, 2);
		memcpy(dst+5, src+11, 2);
		memcpy(dst+8, src+8, 2);
		memcpy(dst+10, src+5, 2);
		memcpy(dst+12, src+2, 2);
	}
	return 0;
}

int get_trans_port(char (*port)[32], int *num)
{
	uint16 maxLen = 256;
	uint16 count = 0;
	int i=0,j=0;
	char *pNext;
	char* portsStr = new char[maxLen];
	char* transStr = new char[maxLen];
	//CString ports; // The human readable port strings (e.g. "USBDBG(100)")
	//CString trans; // The transport option strings (e.g. "SPITRANS=USBDBG SPIPORT=1")

	int32 status = flGetAvailablePorts(&maxLen, portsStr, transStr, &count);
	if( status != TFL_OK && maxLen != 0 )
	{
		// Not enough space - resize the storage
		delete[] portsStr;
		portsStr = new char[maxLen];
		delete[] transStr;
		transStr = new char[maxLen];
		status = flGetAvailablePorts(&maxLen, portsStr, transStr, &count);
	}
	if( status != TFL_OK || count == 0 )
	{
		printf("Error getting ports, or none found");
		delete[] portsStr;
		delete[] transStr;
		return -1;
	}
	char *delim = ",";
	pNext = strtok(portsStr, delim);
	while(pNext != NULL) {
		strcpy(port[i], pNext);
		++i;
		pNext = strtok(NULL, delim);  
     }

	*num = count;
	delete[] portsStr;
	delete[] transStr;

	return 1;

}
void get_key_value(char *key, char *key_value)
{
	GetPrivateProfileString("USER_CONFIG", key, "error", key_value, MAX_PATH, ".\\UserConfig.ini");
}

void set_key_value(char *key, char *key_value)
{
	WritePrivateProfileString("USER_CONFIG", key, key_value, ".\\UserConfig.ini");
}

void initLoadConfig(void)
{
	char temp_key_value[MAX_PATH];
	get_key_value("chipType",temp_key_value);
	g_user_config.chip_type = atoi((const char *)temp_key_value);
	if (g_user_config.chip_type > 1)
		g_user_config.chip_type = 0;

	get_key_value("transType",temp_key_value);
	g_user_config.trans_type = atoi((const char *)temp_key_value);
	if(g_user_config.trans_type > 1)
		g_user_config.trans_type = 0;

	get_key_value("transPort",temp_key_value);
	g_user_config.trans_port = atoi((const char *)temp_key_value);
	if (g_user_config.trans_port > 999999)
		g_user_config.trans_port = 0;

	get_key_value("checkEarse",temp_key_value);
	g_user_config.check_earse = atoi((const char *)temp_key_value);

	get_key_value("checkBurn",temp_key_value);
	g_user_config.check_burn = atoi((const char *)temp_key_value);

	get_key_value("checkSetAddrAndName",temp_key_value);
	g_user_config.check_set_addr_and_name = atoi((const char *)temp_key_value);

	get_key_value("checkGetAddrAndName",temp_key_value);
	g_user_config.check_get_addr_and_name = atoi((const char *)temp_key_value);

	get_key_value("checkAddrAdd",temp_key_value);
	g_user_config.check_addr_auto_add = atoi((const char *)temp_key_value);

	get_key_value("AddrAddCount",temp_key_value);
	g_user_config.addr_auto_add_count = atoi((const char *)temp_key_value);
	if(g_user_config.addr_auto_add_count < 0 || g_user_config.addr_auto_add_count > 2)
		g_user_config.addr_auto_add_count = 0;
	/*get_key_value("checkSetname",temp_key_value);
	g_user_config.check_set_name = atoi((const char *)temp_key_value);

	get_key_value("checkTrim",temp_key_value);
	g_user_config.check_trim = atoi((const char *)temp_key_value);

	get_key_value("trimCap",temp_key_value);
	g_user_config.trim_cap = atoi((const char *)temp_key_value);

	get_key_value("trimFreq",temp_key_value);
	g_user_config.trim_freq = atoi((const char *)temp_key_value);*/

	get_key_value("btaddr", temp_key_value);
	if(-1 != str2hexArray(temp_key_value,(uint8 *)g_user_config.btaddr))
		memcpy(g_user_config.btaddr, temp_key_value, 15);
	else
		strcpy(g_user_config.btaddr, "0002-5b-00ff00");

	get_key_value("btname", temp_key_value);
	memcpy(g_user_config.btname, temp_key_value, MAX_PATH);

	get_key_value("filePath", temp_key_value);
	memcpy(g_user_config.file_path, temp_key_value, MAX_PATH);
}

void initSaveConfig(void)
{
	char temp_key_value[MAX_PATH];
	
	itoa(g_user_config.chip_type, temp_key_value, 10);
	set_key_value("chipType", temp_key_value);

	itoa(g_user_config.trans_type, temp_key_value, 10);
	set_key_value("transType", temp_key_value);

	itoa(g_user_config.trans_port, temp_key_value, 10);
	set_key_value("transPort", temp_key_value);

	itoa(g_user_config.check_earse, temp_key_value, 10);
	set_key_value("checkEarse", temp_key_value);

	itoa(g_user_config.check_burn, temp_key_value, 10);
	set_key_value("checkBurn", temp_key_value);

	itoa(g_user_config.check_set_addr_and_name, temp_key_value, 10);
	set_key_value("checkSetAddrAndName", temp_key_value);

	itoa(g_user_config.check_get_addr_and_name, temp_key_value, 10);
	set_key_value("checkGetAddrAndName", temp_key_value);

	itoa(g_user_config.check_addr_auto_add, temp_key_value, 10);
	set_key_value("checkAddrAdd", temp_key_value);

	if(g_user_config.addr_auto_add_count < 0 || g_user_config.addr_auto_add_count > 2)
	g_user_config.addr_auto_add_count = 0;
	itoa(g_user_config.addr_auto_add_count, temp_key_value, 10);
	set_key_value("AddrAddCount", temp_key_value);
	/*itoa(g_user_config.check_set_name,temp_key_value, 10);
	set_key_value("checkSetname",temp_key_value);

	itoa(g_user_config.check_trim,temp_key_value, 10);
	set_key_value("checkTrim",temp_key_value);

	itoa(g_user_config.trim_cap,temp_key_value, 10);
	set_key_value("trimCap",temp_key_value);

	itoa(g_user_config.trim_freq,temp_key_value, 10);
	set_key_value("trimFreq",temp_key_value);*/

	if(-1 != str2hexArray(g_user_config.btaddr,(uint8 *)temp_key_value))
		memcpy(temp_key_value, g_user_config.btaddr, 15);
	else
		strcpy(temp_key_value, "0002-5b-00ff00");
	set_key_value("btaddr", temp_key_value);

	memcpy(temp_key_value, g_user_config.btname, MAX_PATH);
	set_key_value("btname", temp_key_value);

	memcpy(temp_key_value, g_user_config.file_path, MAX_PATH);
	set_key_value("filePath", temp_key_value);
}

CQCC30xx_toolDlg::CQCC30xx_toolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQCC30xx_toolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQCC30xx_toolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQCC30xx_toolDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CQCC30xx_toolDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE, &CQCC30xx_toolDlg::OnBnClickedButtonOpenFile)
	ON_BN_CLICKED(IDC_BUTTON_GET_PORT, &CQCC30xx_toolDlg::OnBnClickedButtonGetPort)
	ON_BN_CLICKED(IDC_CHECK_EARSE, &CQCC30xx_toolDlg::OnBnClickedCheckEarse)
	ON_BN_CLICKED(IDC_CHECK_BURN, &CQCC30xx_toolDlg::OnBnClickedCheckBurn)
	ON_BN_CLICKED(IDC_CHECK_SET_ADDR_AND_NAME, &CQCC30xx_toolDlg::OnBnClickedCheckSetAddrAndName)
	ON_BN_CLICKED(IDC_CHECK_ADDR_AUTO_ADD, &CQCC30xx_toolDlg::OnBnClickedCheckAddrAutoAdd)
	ON_BN_CLICKED(IDC_CHECK_GET_ADDR_AND_NAME, &CQCC30xx_toolDlg::OnBnClickedCheckGetAddrAndName)
	ON_BN_CLICKED(IDC_CHECK_TRIM, &CQCC30xx_toolDlg::OnBnClickedCheckTrim)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CQCC30xx_toolDlg 消息处理程序
void program_dev_multiple(void)
{
	int retVal = -1;

	// Program 4 devices
	uint32 devMask = 0xff;

	if (flmOpen(devMask, 0, TFL_TRB) != TFL_OK)
	{
		printf("Failed to open devices (%d)", flmGetBitErrorField());
	}
	else
	{
		bool success = true;

		//load firmware
		if (flmReadProgramFiles(".\\dump_R.xuv") != TFL_OK)
		{
			printf("Failed to read flash program files\n");
			success = false;
		}

		if (success && flmSetFlashType(devMask, TFL_TYPE_SQIF) != TFL_OK)
		{
			printf("Failed to set flash type\n");
			success = false;
		}

		if (success && flmSetSubsysBank(devMask, 4, 0) != TFL_OK)
		{
			printf("Failed to set subsystem and bank\n");
			success = false;
		}

		if (success && flmProgramSpawn(devMask, 0, 0, 0) != TFL_OK)
		{
			printf("Failed to spawn flash program thread\n");
			success = false;
		}

		if (success)
		{
			uint16 devicesRunning;
			int32 progress;
			do
			{
				devicesRunning = 0;
				for (uint32 devIndex = 0; devIndex < TFL_MAX_DEVICES; ++devIndex)
				{
					// Only check the progress if the device is in the mask
					if ((devMask >> devIndex) & 1)
					{
						progress = flmGetDeviceProgress(devIndex);
						if (progress < 100)
						{
							++devicesRunning;
						}
						printf("dev %d,progress = %d%%\n", devIndex, progress);
					}
				}
				Sleep(1000);
			} while (devicesRunning > 0);

			printf("Completed\n");

			int32 error = flmGetLastError();
			if (error != TFL_OK)
			{
				printf("Programming failed with error: %d. Failed devices mask %0x\n", error, flmGetBitErrorField());
				success = false;
			}
		}

		if (success)
		{
			printf("Successfully programmed devices\n");
			retVal = 0;
		}

		flmClose(devMask);
	}
}

int CQCC30xx_toolDlg::program_dev_single()
{
	int retVal = -1;
	uint8 m_trans_type;
	if(g_user_config.trans_type)
		m_trans_type = TFL_USBDBG;
	else
		m_trans_type = TFL_TRB;
	if (flOpen(g_user_config.trans_port, 0, 0, m_trans_type) != TFL_OK)
	{
		printf("Failed to open TestFlash connection\n");
	}
	else
	{
		bool success = true;
		//load firmware
		if (flReadProgramFiles(g_user_config.file_path) != TFL_OK)
		{
			printf("Failed to read flash program files\n");
			success = false;
		}

		if (success && flSetFlashType(TFL_TYPE_SQIF) != TFL_OK)
		{
			printf("Failed to set flash type\n");
			success = false;
		}

		if (success && flSetSubsysBank(4, 0) != TFL_OK)
		{
			printf("Failed to set subsystem and bank\n");
			success = false;
		}

		if (success && g_user_config.check_earse)
		{
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("EARSE..."));
			if(flErase() != TFL_OK){
				printf("Failed to erase\n");
				success = false;
			}else{
				printf("erase success\n");
			}
		}

		if (success && g_user_config.check_burn)
		{
			GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("DOWNLOAD..."));
			if(flProgramSpawn() != TFL_OK) {
				printf("Failed to spawn flash program thread\n");
				success = false;
			}else{
				printf("spawn flash program thread start...\n");
			}
		}

		if (success)
		{
			int32 progress = 0;//flGetProgress();
			CProgressCtrl* pProg = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_PROCESS);
			pProg->SetRange(0, 100);
			pProg->SetPos(0);
			while (progress < 100)
			{
				progress = flGetProgress();
				printf("Programming...%d%%\n", progress);
				pProg->SetPos(progress);

				Sleep(1000);
				//也可关联控件变量
			}
			pProg->SetPos(100);
			printf("Completed\n");

			int32 error = flGetLastError();
			if (error != TFL_OK)
			{
				printf("Programming failed with error: %d\n", error);
				success = false;
			}
			pProg->SetPos(0);
		}

		if (success)
		{
			printf("Successfully programmed device\n");
			retVal = 0;
		}

		flClose();
	}

	return retVal;

}

int CQCC30xx_toolDlg::config_operation()
{
	int32 ret = -1;
	int i = 0;
	uint32 devHandle;
	uint16 trans_port_type;
	char trans_port_num[32];
	if(0 == g_user_config.trans_type){
		trans_port_type = TRB;
	}else if(1 == g_user_config.trans_type){
		trans_port_type = USBDBG;
	}else{
		return -1;
	}
	itoa(g_user_config.trans_port, trans_port_num, 10);
	devHandle = openTestEngine(trans_port_type, (const char *)trans_port_num, 0, 5000, 0);

	if (devHandle != 0)
	{
		uint16 unused = 0;
		bool success = true;
		// Initialise the configuration cache
		if(g_user_config.chip_type)
			success = teConfigCacheInit(devHandle, CFG_DB_512X);
		else
			success = teConfigCacheInit(devHandle, CFG_DB_514X);

		// Read the configuration into the cache from the device
		if (success == TE_OK) {
			success = teConfigCacheRead(devHandle, NULL, unused);
		}
#if 0
		// Read current XTAL trim value
		if (success == TE_OK) {
			char valueString[KEY_READ_BUFFER_LEN];
			uint32 maxLen = KEY_READ_BUFFER_LEN;

			success = teConfigCacheReadItem(devHandle, "curator:XtalFreqTrim", valueString, &maxLen);
			if (success == TE_OK) {
				printf("curator:XtalFreqTrim = %s", valueString);
			}
		}

		// Write updated XTAL trim value
		if (success == TE_OK) {
			success = teConfigCacheWriteItem(devHandle, "curator15:XtalFreqTrim", "3");
		}
#endif

		// Write Bluetooth address
		GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("SET CONFIG..."));
		if (success == TE_OK && g_user_config.check_set_addr_and_name) {
			char valueString[KEY_READ_BUFFER_LEN];
			uint8 m_addr[6];
			if(-1 != str2hexArray(g_user_config.btaddr, m_addr)) {
				if(g_user_config.chip_type) {
					sprintf(valueString, "{0x%02x%02x%02x,0x%02x,0x%02x%02x}",m_addr[3],m_addr[4],m_addr[5],m_addr[2],m_addr[0],m_addr[1]);
					success = teConfigCacheWriteItem(devHandle, "bt2:PSKEY_BDADDR", valueString);//"{0xff73,0x95,0xB49A}");
				} else {
					sprintf(valueString, "[ %02x %02x %02x %02x %02x %02x ]",m_addr[5],m_addr[4],m_addr[3],m_addr[2],m_addr[1],m_addr[0]);
					success = teConfigCacheWriteItem(devHandle, "bt2:BD_ADDRESS", valueString);//"[ 71 ff 00 95 9a b4 ]");
				}
			}else {
				success = false;
			}
		}

		// Write Bluetooth name
		if (success == TE_OK && g_user_config.check_set_addr_and_name) {
			char valueString[KEY_READ_BUFFER_LEN];
			sprintf(valueString, "\"%s\"", g_user_config.btname);
			if(g_user_config.chip_type) {
				//qcc512x
				success = teConfigCacheWriteItem(devHandle, "bt2:PSKEY_DEVICE_NAME", valueString);
			} else {
				success = teConfigCacheWriteItem(devHandle, "app5:DeviceName", valueString);
			}
		}

		////Read Bluetooth addr
		GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("GET CONFIG..."));
		if (success == TE_OK && g_user_config.check_get_addr_and_name) {
			char valueString[KEY_READ_BUFFER_LEN];
			uint32 maxLen = KEY_READ_BUFFER_LEN;
			if(g_user_config.chip_type)
				success = teConfigCacheReadItem(devHandle, "bt2:PSKEY_BDADDR", valueString, &maxLen);//QCC512X
			else
				success = teConfigCacheReadItem(devHandle, "bt2:BD_ADDRESS", valueString, &maxLen);//QCC514X
			if (success == TE_OK) {
				CString str;
				char tmp_addr[15];
				printf("dev addr = %s", valueString);

				addr2str(valueString, tmp_addr, g_user_config.chip_type);
				str.Format("%s", tmp_addr);
				GetDlgItem(IDC_EDIT_ADDR)->SetWindowText(str);
			}
		}

		//Read Bluetooth name
		if (success == TE_OK && g_user_config.check_get_addr_and_name) {
			char valueString[KEY_READ_BUFFER_LEN];
			uint32 maxLen = KEY_READ_BUFFER_LEN;
			if(g_user_config.chip_type)
				success = teConfigCacheReadItem(devHandle, "bt2:PSKEY_DEVICE_NAME", valueString, &maxLen);
			else
				success = teConfigCacheReadItem(devHandle, "app5:DeviceName", valueString, &maxLen);
			if (success == TE_OK) {
				CString str;
				int i=0,j=0;
				while('\0' != valueString[i++]){
					if(0x34/*"*/ != valueString[i]){
						valueString[j++] = valueString[i];
					}
				}
				valueString[j-2] = '\0';
				printf("devname = %s", valueString);
				str.Format("%s", valueString);
				GetDlgItem(IDC_EDIT_DEV_NAME)->SetWindowText(str);
			}
		}

		// Write the configuration cache to the device
		if (success == TE_OK) {
			ret = 0;
			success = teConfigCacheWrite(devHandle, NULL, unused);
		} else {
			ret = -1;
		}

		closeTestEngine(devHandle);
	}

	return ret;
}

BOOL CQCC30xx_toolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	//InitConsoleWindows();

	m_editFont.CreatePointFont(180, _T("宋体"));
	GetDlgItem(IDC_EDIT_ADDR)->SetFont(&m_editFont);
	GetDlgItem(IDC_EDIT_DEV_NAME)->SetFont(&m_editFont);
	GetDlgItem(IDC_BUTTON_START)->SetFont(&m_editFont);

	m_editFont1.CreatePointFont(250, _T("宋体"));
	GetDlgItem(IDC_STATIC_RESULT)->SetFont(&m_editFont1);

	// TODO: 在此添加额外的初始化代码
	initLoadConfig();
	setConfigToDialog();

	GetDlgItem(IDC_CHECK_TRIM)->EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CQCC30xx_toolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CQCC30xx_toolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CQCC30xx_toolDlg::loadConfigFromDialog()
{
	CString str;

	g_user_config.check_earse =((CButton *)GetDlgItem(IDC_CHECK_EARSE))->GetCheck();

	g_user_config.check_burn =((CButton *)GetDlgItem(IDC_CHECK_BURN))->GetCheck();

	g_user_config.check_set_addr_and_name =((CButton *)GetDlgItem(IDC_CHECK_SET_ADDR_AND_NAME))->GetCheck();

	g_user_config.check_get_addr_and_name =((CButton *)GetDlgItem(IDC_CHECK_GET_ADDR_AND_NAME))->GetCheck();

	g_user_config.check_addr_auto_add =((CButton *)GetDlgItem(IDC_CHECK_ADDR_AUTO_ADD))->GetCheck();

	g_user_config.chip_type = ((CComboBox *)GetDlgItem(IDC_COMBO_CHIP_TYPE))->GetCurSel();

	g_user_config.trans_type = ((CComboBox *)GetDlgItem(IDC_COMBO_PORT_TYPE))->GetCurSel();

	GetDlgItem(IDC_EDIT_FILE_PATH)->GetWindowText(str);
	memcpy(g_user_config.file_path, str.GetBuffer(str.GetLength()),str.GetLength());
	g_user_config.file_path[str.GetLength()] = '\0';

	GetDlgItem(IDC_EDIT_PROT)->GetWindowText(str);
	g_user_config.trans_port = _ttoi(str);

	GetDlgItem(IDC_EDIT_ADDR)->GetWindowText(str);
	memcpy(g_user_config.btaddr, str.GetBuffer(str.GetLength()),14);
	g_user_config.btaddr[15] = '\0';

	GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->GetWindowText(str);
	g_user_config.addr_auto_add_count = _ttoi(str);
	if(g_user_config.addr_auto_add_count < 0 || g_user_config.addr_auto_add_count > 2){
		g_user_config.addr_auto_add_count = 0;
		GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->SetWindowText(_T("0"));
	}

	GetDlgItem(IDC_EDIT_DEV_NAME)->GetWindowText(str);
	memcpy(g_user_config.btname, str.GetBuffer(str.GetLength()),str.GetLength());
	g_user_config.btname[str.GetLength()] = '\0';
}

void CQCC30xx_toolDlg::setConfigToDialog()
{
	CString str;

	((CButton *)GetDlgItem(IDC_CHECK_EARSE))->SetCheck(g_user_config.check_earse);

	((CButton *)GetDlgItem(IDC_CHECK_BURN))->SetCheck(g_user_config.check_burn);

	((CButton *)GetDlgItem(IDC_CHECK_SET_ADDR_AND_NAME))->SetCheck(g_user_config.check_set_addr_and_name);
	if(!g_user_config.check_set_addr_and_name){
		GetDlgItem(IDC_EDIT_ADDR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEV_NAME)->EnableWindow(FALSE);
	}

	((CButton *)GetDlgItem(IDC_CHECK_GET_ADDR_AND_NAME))->SetCheck(g_user_config.check_get_addr_and_name);

	((CButton *)GetDlgItem(IDC_CHECK_ADDR_AUTO_ADD))->SetCheck(g_user_config.check_addr_auto_add);
	if(!g_user_config.check_addr_auto_add){
		GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->EnableWindow(FALSE);
	}

	((CComboBox *)GetDlgItem(IDC_COMBO_CHIP_TYPE))->InsertString(0,_T("QCC514X"));
	((CComboBox *)GetDlgItem(IDC_COMBO_CHIP_TYPE))->InsertString(1,_T("QCC512X"));

	((CComboBox *)GetDlgItem(IDC_COMBO_PORT_TYPE))->InsertString(0,_T("TRB"));
	((CComboBox *)GetDlgItem(IDC_COMBO_PORT_TYPE))->InsertString(1,_T("USBDBG"));

	((CComboBox *)GetDlgItem(IDC_COMBO_CHIP_TYPE))->SetCurSel(g_user_config.chip_type);

	((CComboBox *)GetDlgItem(IDC_COMBO_PORT_TYPE))->SetCurSel(g_user_config.trans_type);

	str.Format("%s", g_user_config.file_path);
	GetDlgItem(IDC_EDIT_FILE_PATH)->SetWindowText(str);

	str.Format("%d", g_user_config.trans_port);
	GetDlgItem(IDC_EDIT_PROT)->SetWindowText(str);

	str.Format("%s", g_user_config.btaddr);
	GetDlgItem(IDC_EDIT_ADDR)->SetWindowText(str);

	str.Format("%d", g_user_config.addr_auto_add_count);
	GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->SetWindowText(str);

	str.Format("%s",g_user_config.btname);
	GetDlgItem(IDC_EDIT_DEV_NAME)->SetWindowText(str);
}

void CQCC30xx_toolDlg::enableAllControl(int enable)
{
	if (enable) {
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_EARSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_OPEN_FILE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_SET_ADDR_AND_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ADDR_AUTO_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_BURN)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_GET_PORT)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_GET_ADDR_AND_NAME)->EnableWindow(TRUE);

		GetDlgItem(IDC_COMBO_CHIP_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_PORT_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FILE_PATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROT)->EnableWindow(TRUE);
		if(g_user_config.check_set_addr_and_name){
			GetDlgItem(IDC_EDIT_ADDR)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_DEV_NAME)->EnableWindow(TRUE);
		}
		if(g_user_config.check_addr_auto_add){
			GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->EnableWindow(TRUE);
		}
	} else {
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_EARSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OPEN_FILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SET_ADDR_AND_NAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_ADDR_AUTO_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_BURN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_GET_PORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_GET_ADDR_AND_NAME)->EnableWindow(FALSE);

		GetDlgItem(IDC_COMBO_CHIP_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PORT_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FILE_PATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ADDR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEV_NAME)->EnableWindow(FALSE);
	}
}

DWORD WINAPI download_config_proc(void *pParam)
{
	CQCC30xx_toolDlg *mDlg = (CQCC30xx_toolDlg *)pParam;

	int result = 0;
	mDlg->enableAllControl(FALSE);
	mDlg->loadConfigFromDialog();
	(mDlg->GetDlgItem(IDC_STATIC_RESULT))->SetWindowText(_T("IDLE"));
	if(g_user_config.check_earse || g_user_config.check_burn) {
		result = mDlg->program_dev_single();
		if(-1 == result)
			(mDlg->GetDlgItem(IDC_STATIC_RESULT))->SetWindowText(_T("DOWNLOAD FAIL"));
	}
	
	if(-1 != result && (g_user_config.check_set_addr_and_name || g_user_config.check_get_addr_and_name)) {
		Sleep(1000);
		result = mDlg->config_operation();
		if(-1 == result)
			(mDlg->GetDlgItem(IDC_STATIC_RESULT))->SetWindowText(_T("CONFIG FAIL"));
	}

	if(-1 != result) {
		(mDlg->GetDlgItem(IDC_STATIC_RESULT))->SetWindowText(_T("SUCCESS"));
		if(g_user_config.check_set_addr_and_name && g_user_config.check_addr_auto_add) {
			uint8 m_addr[6];
			CString str;
			if(g_user_config.addr_auto_add_count > 2)
				g_user_config.addr_auto_add_count = 0;

			if(-1 != str2hexArray(g_user_config.btaddr,m_addr)){
				uint32 tmp = m_addr[3]<<16 | m_addr[4]<<8 | m_addr[5];
				tmp = (tmp + g_user_config.addr_auto_add_count) & 0xffffff;
				sprintf(g_user_config.btaddr,"%02x%02x-%02x-%06x",m_addr[0],m_addr[1],m_addr[2],tmp);
			}
			str.Format("%s",g_user_config.btaddr);
			(mDlg->GetDlgItem(IDC_EDIT_ADDR))->SetWindowText(str);
			//str.Format("%d",g_user_config.check_addr_auto_add);
			//GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->SetWindowText(str);
		}
	}
	mDlg->enableAllControl(TRUE);
	return 0;
}

void CQCC30xx_toolDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码
	//program_dev();

	HANDLE hThread;
	DWORD dwThreadId;

	//调用线程设置对话框显示内容
	hThread = CreateThread(NULL	// 默认安全属性
		, NULL		// 默认堆栈大小
		, download_config_proc // 线程入口地址
		, this	//传递给线程函数的参数
		, 0		// 指定线程立即运行
		, &dwThreadId	//线程ID号
		);
	CloseHandle(hThread);	//关闭线程句柄，内核引用计数减一
	//int result = 0;
	//enableAllControl(FALSE);
	//loadConfigFromDialog();
	//GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("IDLE"));
	//if(g_user_config.check_earse || g_user_config.check_burn)
	//	result = program_dev_single();
	//if(-1 != result && (g_user_config.check_set_addr_and_name || g_user_config.check_get_addr_and_name))
	//	result = config_operation();
	//enableAllControl(TRUE);
	//if(-1 == result) {
	//	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("FAIL"));
	//} else {
	//	GetDlgItem(IDC_STATIC_RESULT)->SetWindowText(_T("SUCCESS"));
	//	if(g_user_config.check_set_addr_and_name && g_user_config.check_addr_auto_add) {
	//		uint8 m_addr[6];
	//		CString str;
	//		if(g_user_config.addr_auto_add_count > 2)
	//			g_user_config.addr_auto_add_count = 0;

	//		if(-1 != str2hexArray(g_user_config.btaddr,m_addr)){
	//			uint32 tmp = m_addr[3]<<16 | m_addr[4]<<8 | m_addr[5];
	//			tmp = (tmp + g_user_config.addr_auto_add_count) & 0xffffff;
	//			sprintf(g_user_config.btaddr,"%02x%02x-%02x-%06x",m_addr[0],m_addr[1],m_addr[2],tmp);
	//		}
	//		str.Format("%s",g_user_config.btaddr);
	//		GetDlgItem(IDC_EDIT_ADDR)->SetWindowText(str);
	//		//str.Format("%d",g_user_config.check_addr_auto_add);
	//		//GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->SetWindowText(str);
	//	}
	//}
}

void CQCC30xx_toolDlg::OnBnClickedButtonOpenFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CString filename;//保存路径
	CFileDialog opendlg (TRUE,_T("*"),_T(""),OFN_OVERWRITEPROMPT, _T("All files(*.*)|*.*|XUV files(*.xuv)|*.xuv||"),NULL);
	if (opendlg.DoModal()==IDOK)
	{
		filename=opendlg.GetPathName();
	}
	GetDlgItem(IDC_EDIT_FILE_PATH)->SetWindowText(filename);

	memcpy(g_user_config.file_path,filename.GetBuffer(filename.GetLength()),filename.GetLength());
	g_user_config.file_path[filename.GetLength()] = '\0';
}


DWORD WINAPI draw_port_page(void *pParam)
{
	portChoice *mDlg = (portChoice *)pParam;
	int count;

	Sleep(10);//等待sheet.DoModal();完成

	get_trans_port(g_trans_port_arr, &count);
	((CComboBox *)(mDlg->GetDlgItem(IDC_DIALOG_PORT_COMBO_TYPE)))->InsertString(0, _T("DEBUG"));
	((CComboBox *)(mDlg->GetDlgItem(IDC_DIALOG_PORT_COMBO_TYPE)))->SetCurSel(0);
	for(int i = 0;i<count;i++){
		CString str;
		str.Format("%s",g_trans_port_arr[i]);
		((CComboBox *)(mDlg->GetDlgItem(IDC_DIALOG_PORT_COMBO_PORT)))->InsertString(i, str);
	}
	((CComboBox *)(mDlg->GetDlgItem(IDC_DIALOG_PORT_COMBO_PORT)))->SetCurSel(0);

	return 0;
}

void CQCC30xx_toolDlg::OnBnClickedButtonGetPort()
{
	// TODO: 在此添加控件通知处理程序代码
	INT_PTR nRes;// <用于保存DoModal函数的返回值> 
	portChoice portChoiceDlg;
	HANDLE hThread;
	DWORD dwThreadId;

	//调用线程设置对话框显示内容
	hThread = CreateThread(NULL	// 默认安全属性
		, NULL		// 默认堆栈大小
		, draw_port_page // 线程入口地址
		, &portChoiceDlg	//传递给线程函数的参数
		, 0		// 指定线程立即运行
		, &dwThreadId	//线程ID号
		);
	CloseHandle(hThread);	//关闭线程句柄，内核引用计数减一

	nRes = portChoiceDlg.DoModal();
	
	//portChoiceDlg->GetDlgItem(IDC_DIALOG_PORT_COMBO_PORT);
	if (IDOK == nRes)// <判断对话框退出后返回值是否为IDOK，则继续向下执行，否则继续向下执行>   
	{
		//此处添加点击子对话框确认按钮之后要执行的代码
		char *delim = "(";
		char *delim2 = ")";
		char *pNext = NULL;
		CString str;

		if(NULL != strstr(g_trans_port_arr[portChoiceDlg.port_index],"TRB")){
			g_user_config.trans_type = 0;
		} else if(NULL != strstr(g_trans_port_arr[portChoiceDlg.port_index],"USBDBG")){
			g_user_config.trans_type = 1;
		}

		pNext = strtok(g_trans_port_arr[portChoiceDlg.port_index], delim);
		pNext = strtok(NULL, delim2);  
		if(pNext != NULL) {
			g_user_config.trans_port = atoi(pNext);
		}
		((CComboBox *)GetDlgItem(IDC_COMBO_PORT_TYPE))->SetCurSel(g_user_config.trans_type);
		str.Format("%d", g_user_config.trans_port);
		GetDlgItem(IDC_EDIT_PROT)->SetWindowText(str);
	}
}

void CQCC30xx_toolDlg::OnBnClickedCheckEarse()
{
	// TODO: 在此添加控件通知处理程序代码
	g_user_config.check_earse =((CButton *)GetDlgItem(IDC_CHECK_EARSE))->GetCheck();
}


void CQCC30xx_toolDlg::OnBnClickedCheckBurn()
{
	// TODO: 在此添加控件通知处理程序代码
	g_user_config.check_burn =((CButton *)GetDlgItem(IDC_CHECK_BURN))->GetCheck();
}


void CQCC30xx_toolDlg::OnBnClickedCheckSetAddrAndName()
{
	// TODO: 在此添加控件通知处理程序代码
	g_user_config.check_set_addr_and_name =((CButton *)GetDlgItem(IDC_CHECK_SET_ADDR_AND_NAME))->GetCheck();
	
	if(g_user_config.check_set_addr_and_name){
		GetDlgItem(IDC_EDIT_ADDR)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_DEV_NAME)->EnableWindow(TRUE);
	} else {
		GetDlgItem(IDC_EDIT_ADDR)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_DEV_NAME)->EnableWindow(FALSE);
	}
}


void CQCC30xx_toolDlg::OnBnClickedCheckAddrAutoAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	g_user_config.check_addr_auto_add =((CButton *)GetDlgItem(IDC_CHECK_ADDR_AUTO_ADD))->GetCheck();

	if(g_user_config.check_addr_auto_add){
		GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->EnableWindow(TRUE);
	} else {
		GetDlgItem(IDC_EDIT_ADDR_ADD_COUNT)->EnableWindow(FALSE);
	}
}


void CQCC30xx_toolDlg::OnBnClickedCheckGetAddrAndName()
{
	// TODO: 在此添加控件通知处理程序代码
	g_user_config.check_get_addr_and_name =((CButton *)GetDlgItem(IDC_CHECK_GET_ADDR_AND_NAME))->GetCheck();
}


void CQCC30xx_toolDlg::OnBnClickedCheckTrim()
{
	// TODO: 在此添加控件通知处理程序代码
}


HBRUSH CQCC30xx_toolDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	// TODO:  在此更改 DC 的任何特性
	if(IDC_STATIC_RESULT == pWnd->GetDlgCtrlID())
	{
		CString str;
		GetDlgItem(IDC_STATIC_RESULT)->GetWindowText(str);
		if(_T("SUCCESS") == str){
			pDC->SetTextColor(RGB(0,255,0));//设置字体颜色
		}else if(_T("DOWNLOAD FAIL") == str || _T("CONFIG FAIL") == str){
			pDC->SetTextColor(RGB(255,0,0));
		}else{
			pDC->SetTextColor(RGB(0,0,255));
		}
		//pDC->SetBkColor(RGB(0, 255, 0));//设置背景颜色
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
