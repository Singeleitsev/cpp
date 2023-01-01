// Этот исходный код примеров MFC демонстрирует функционирование пользовательского интерфейса Fluent на основе MFC в Microsoft Office
// ("Fluent UI") и предоставляется исключительно как справочный материал в качестве дополнения к
// справочнику по пакету Microsoft Foundation Classes и связанной электронной документации,
// включенной в программное обеспечение библиотеки MFC C++.
// Условия лицензионного соглашения на копирование, использование или распространение Fluent UI доступны отдельно.
// Для получения дополнительных сведений о нашей программе лицензирования Fluent UI посетите веб-сайт
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// (C) Корпорация Майкрософт (Microsoft Corp.)
// Все права защищены.

// MFCApplication1.cpp: определяет поведение классов для приложения.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MFCApplication1.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "IpFrame.h"
#include "MFCApplication1Doc.h"
#include "MFCApplication1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBIM23App

BEGIN_MESSAGE_MAP(CBIM23App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CBIM23App::OnAppAbout)
	// Стандартные команды по работе с файлами документов
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Стандартная команда настройки печати
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// Создание CBIM23App

CBIM23App::CBIM23App() noexcept
{
	m_bHiColorIcons = TRUE;


	m_nAppLook = 0;
	// поддержка диспетчера перезагрузки
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// Если приложение построено с поддержкой среды Common Language Runtime (/clr):
	//     1) Этот дополнительный параметр требуется для правильной поддержки работы диспетчера перезагрузки.
	//   2) В своем проекте для сборки необходимо добавить ссылку на System.Windows.Forms.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: замените ниже строку идентификатора приложения строкой уникального идентификатора; рекомендуемый
	// формат для строки: ИмяКомпании.ИмяПродукта.СубПродукт.СведенияОВерсии
	SetAppID(_T("MFCApplication1.AppID.NoVersion"));

	// TODO: добавьте код создания,
	// Размещает весь важный код инициализации в InitInstance
}

// Единственный объект CBIM23App

CBIM23App theApp;
// Этот идентификатор создан уникальным для вашего приложения
// Его можно изменить, если вы предпочитаете использовать какой-то определенный идентификатор

// {59425141-76d8-4dd2-a51a-e2b5c37f77f4}
static const CLSID clsid =
{0x59425141,0x76d8,0x4dd2,{0xa5,0x1a,0xe2,0xb5,0xc3,0x7f,0x77,0xf4}};

const GUID CDECL _tlid = {0x6b3ca93f,0xd524,0x40dd,{0xbd,0x36,0x37,0xba,0x2e,0xd4,0xe2,0x6b}};
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// Инициализация CBIM23App

BOOL CBIM23App::InitInstance()
{
	// InitCommonControlsEx() требуются для Windows XP, если манифест
	// приложения использует ComCtl32.dll версии 6 или более поздней версии для включения
	// стилей отображения.  В противном случае будет возникать сбой при создании любого окна.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Выберите этот параметр для включения всех общих классов управления, которые необходимо использовать
	// в вашем приложении.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Инициализация библиотек OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// Для использования элемента управления RichEdit требуется метод AfxInitRichEdit2()
	// AfxInitRichEdit2();

	// Стандартная инициализация
	// Если эти возможности не используются и необходимо уменьшить размер
	// конечного исполняемого файла, необходимо удалить из следующего
	// конкретные процедуры инициализации, которые не требуются
	// Измените раздел реестра, в котором хранятся параметры
	// TODO: следует изменить эту строку на что-нибудь подходящее,
	// например на название организации
	SetRegistryKey(_T("Локальные приложения, созданные с помощью мастера приложений"));
	LoadStdProfileSettings(8);  // Загрузите стандартные параметры INI-файла (включая MRU)


	InitContextMenuManager();
	InitShellManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Зарегистрируйте шаблоны документов приложения.  Шаблоны документов
	//  выступают в роли посредника между документами, окнами рамок и представлениями
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_MFCApplication1TYPE,
		RUNTIME_CLASS(CBIM23Doc),
		RUNTIME_CLASS(CChildFrame), // настраиваемая дочерняя рамка MDI
		RUNTIME_CLASS(CBIM23View));
	if (!pDocTemplate)
		return FALSE;
	pDocTemplate->SetServerInfo(
		IDR_MFCApplication1TYPE_SRVR_EMB, IDR_MFCApplication1TYPE_SRVR_IP,
		RUNTIME_CLASS(CInPlaceFrame));
	AddDocTemplate(pDocTemplate);
	// Подключите COleTemplateServer к шаблону документа
	//  COleTemplateServer создает новые документы от имени
	//  контейнеров OLE, выполняющих запрос с помощью информации,
	//  указанной в шаблоне документа
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);
	// Зарегистрируйте все фабрики сервера OLE как работающие.  Это позволит
	//  библиотекам OLE создавать объекты из других приложений
	COleTemplateServer::RegisterAll();
		// Примечание. Приложения MDI регистрируют все объекты сервера вне зависимости от наличия параметра
		//  /Embedding или /Automation в командной строке

	// создайте главное окно рамки MDI
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// вызов DragAcceptFiles только при наличии суффикса
	//  В приложении MDI это должно произойти сразу после задания m_pMainWnd
	// Включить открытие перетаскивания
	m_pMainWnd->DragAcceptFiles();

	// Разрешить использование расширенных символов в горячих клавишах меню
	CMFCToolBar::m_bExtCharTranslation = TRUE;

	// Синтаксический разбор командной строки на стандартные команды оболочки, DDE, открытие файлов
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Включить открытие выполнения DDE
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Приложение было запущено с параметром /Embedding или /Automation.
	// Запуск приложения как сервера автоматизации.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Не показывать главное окно
		return TRUE;
	}
	// Приложение было запущено с параметром /Register или /Unregister.  Отключите
	// библиотеку типов.  Иначе произойдет отмена регистрации в ProcessShellCommand().
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		UnregisterShellFileTypes();
		m_server.UpdateRegistry(OAT_DOC_OBJECT_SERVER, nullptr, nullptr, FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
	}
	// Приложение было запущено отдельно или с другими параметрами (например, /Register
	// или /Regserver).  Обновите записи реестра, включая библиотеку типов.
	else
	{
		m_server.UpdateRegistry(OAT_DOC_OBJECT_SERVER);
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
	}

	// Команды диспетчеризации, указанные в командной строке.  Значение FALSE будет возвращено, если
	// приложение было запущено с параметром /RegServer, /Register, /Unregserver или /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// Главное окно было инициализировано, поэтому отобразите и обновите его
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CBIM23App::ExitInstance()
{
	//TODO: обработайте дополнительные ресурсы, которые могли быть добавлены
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// Обработчики сообщений CBIM23App


// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Команда приложения для запуска диалога
void CBIM23App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// Методы загрузки или сохранения настроек CBIM23App

void CBIM23App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CBIM23App::LoadCustomState()
{
}

void CBIM23App::SaveCustomState()
{
}

// Обработчики сообщений CBIM23App



