#include <cvirte.h>
#include <userint.h>
#include "TestExec.h"	// UIR header
#include "tsutil.h"		// Helpful CVI functions for TestStand
#include "tsui.h"		// API's for the TestStand ActiveX controls
#include "tsuisupp.h"
#include <formatio.h>

/*This example shows the ability to process custom command line arguments.
This example is the same as the TestStand Basic User Interface example, but has been modified to take additional command line arguments
This example was also modified to return error information via the console instead of message popups
*/

// this structure holds specific command line arguments to pass to the executing sequence
typedef struct
{
	long 	Keyboard;
	long 	Video;
	VBOOL 	Power;
	VBOOL 	CPU;
	VBOOL 	ROM;
	VBOOL 	RAM;

} SimulatedFailures;

static SimulatedFailures fails = {.Keyboard = 6, .Video = 5};

// this structure holds the handles to the objects that make up an application window
typedef struct
{
	int			panel;
	CAObjHandle	engine;

	// ActiveX control handles:
	CAObjHandle applicationMgr;
	CAObjHandle sequenceFileViewMgr;
	CAObjHandle executionViewMgr;

	CAObjHandle openFileBtn;
	CAObjHandle runSelectedBtn;
	CAObjHandle tracingEnabledCheckBox;
	CAObjHandle sequenceView;
	CAObjHandle label;
	CAObjHandle sequenceListBox;


} MainPanel;

static MainPanel 	gMainWindow;	// the application only has one window

// the presence of these two variables is expected by the tsErrChk macro from tsutil.h.  Usually you declare these variables as locals
// in each function that uses tsErrChk. However, since all the code in this file runs in a single thread, they can be globals for convenience
ERRORINFO	errorInfo = {0, 0, "", "", "", 0, 0};
ErrMsg		errMsg = "";

static int 	SetupActiveXControls(void);
static int	ExitApplication(void);
static void DisplayError(int errorCode);
static void ClearErrorMessage(void);
static int 	MainCallback(int panelOrMenuBarHandle, int controlOrMenuItemID, int event, void *callbackData, int eventData1, int eventData2);

//------------------------------------------------------
//	  MAIN
//------------------------------------------------------

int main(int argc, char *argv[])
{
	int		error = 0;
	long	exitCode = 0;

	nullChk( InitCVIRTE(0, argv, 0));	// initialize CVI runtime engine

	// load the panel for the main window from the .UIR file
	errChk( gMainWindow.panel = LoadPanelEx (0, "TestExec.uir", MAINPANEL, __CVIUserHInst));

	// get ActiveX ctrl handles, register ActiveX event callbacks, and connect TestStand controls
	errChk( SetupActiveXControls());

	errChk( InstallMainCallback(MainCallback, 0, 0));	// handle the EVENT_END_TASK event

	// make a handle to engine conveniently accessible
	tsErrChk( TSUI_ApplicationMgrGetEngine(gMainWindow.applicationMgr, &errorInfo, &gMainWindow.engine));

	// start up the TestStand User Interface Components. this also logs in the user
	tsErrChk( TSUI_ApplicationMgrStart(gMainWindow.applicationMgr, &errorInfo));

	// display window and process user input until application exits
	errChk( DisplayPanel(gMainWindow.panel));
	errChk( RunUserInterface());

	errChk( TSUI_ApplicationMgrGetExitCode(gMainWindow.applicationMgr, &errorInfo, &exitCode));

Error:

	if (gMainWindow.panel > 0)
		DiscardPanel(gMainWindow.panel);

	DisplayError(error);

	return exitCode;
}

//------------------------------------------------------
//	  CVI CALLBACKS
//------------------------------------------------------

int CVICALLBACK MainPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:		// EVENT_CLOSE == user clicked on window close box
			ExitApplication(); 	// this function displays error, if any
	}

	return 0;
}

static int MainCallback(int panelOrMenuBarHandle, int controlOrMenuItemID, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_END_TASK:	// EVENT_END_TASK can occur when windows shuts down or when the user selects Close from the context menu for the application's task bar item
			if (!ExitApplication() && !eventData1)
				return 1; //  don't immediately exit if we have cleanup to do and the whole computer is not shutting down
			break;
	}

	return 0;
}

//------------------------------------------------------
//	  TESTSTAND EVENT CALLBACKS
//------------------------------------------------------

// the ApplicationMgr control sends this event when it is ok to exit the application.
// discard any handles to TestStand objects here at the latest
HRESULT CVICALLBACK ApplicationMgr_OnExitApplication(CAObjHandle caServerObjHandle, void *caCallbackData)
{
	CA_DiscardObjHandle(gMainWindow.engine);
	gMainWindow.engine = 0;

	ExitApplication();
	return S_OK;
}


// the ApplicationMgr sends this event when the TestStand UI Controls need to display an error
HRESULT CVICALLBACK ApplicationMgr_OnReportError(CAObjHandle caServerObjHandle, void *caCallbackData, long  errorCode, char *errorMessage)
{
	strncpy(errMsg, errorMessage, 1024);	// update global errMsg buffer
	DisplayError(errorCode);
	return S_OK;;
}

// the ApplicationMgr sends this event to request that the UI display a particular execution
HRESULT CVICALLBACK ApplicationMgr_OnDisplayExecution(CAObjHandle caServerObjHandle, void *caCallbackData, TSUIObj_Execution execution, enum TSUIEnum_ExecutionDisplayReasons reason)
{
	int	error = 0;

	// bring application to front if we hit a breakpoint
	if (reason == TSUIConst_ExecutionDisplayReason_Breakpoint || reason == TSUIConst_ExecutionDisplayReason_BreakOnRunTimeError)
		errChk( SetActivePanel(gMainWindow.panel));

	tsErrChk( TSUI_ExecutionViewMgrSetByRefExecution(gMainWindow.executionViewMgr, &errorInfo, execution));

Error:
	DisplayError(error);
	return error < 0 ? E_FAIL : S_OK;
}

// the ApplicationMgr sends this event to request that the UI display a particular sequence file
HRESULT CVICALLBACK ApplicationMgr_OnDisplaySequenceFile(CAObjHandle caServerObjHandle, void *caCallbackData, TSUIObj_SequenceFile file, enum TSUIEnum_SequenceFileDisplayReasons reason)
{
	int	error = 0;

	tsErrChk( TSUI_SequenceFileViewMgrSetByRefSequenceFile(gMainWindow.sequenceFileViewMgr, &errorInfo, file));

Error:
	DisplayError(error);
	return error < 0 ? E_FAIL : S_OK;
}

// the ApplicationMgr sends this event when validating and processing command-line arguments that it does not recognize
HRESULT CVICALLBACK ApplicationMgr_OnProcessUserCommandLineArguments(CAObjHandle caServerObjHandle, void *caCallbackData, VBOOL  processCommand, TSUIObj_Strings  arguments, long *currentArgument, enum TSUIEnum_ProcessCommandLineErrors *errorProcessing, char **errorMessage)
{
	int 	error = 0;
	long 	numArgs = 0;
	int 	offset = 0;
	char 	*currentArg = NULL;
	char 	**simFails = NULL;
	
	tsErrChk(TSUISUPP_StringsGetCount ((CAObjHandle)arguments, &errorInfo, &numArgs)); 
	tsErrChk(TSUISUPP_StringsGetItem ((CAObjHandle)arguments, &errorInfo, *currentArgument, &currentArg));
	
	if (strcmp(currentArg, "/SimulateFailure") == 0)
	{
		*currentArgument += 1;
		offset = numArgs - *currentArgument;
		simFails = calloc (numArgs-*currentArgument, sizeof(char*));
		
		for (int i = 0; i  < offset; i++)
		{
			tsErrChk(TSUISUPP_StringsGetItem ((CAObjHandle)arguments, &errorInfo, *currentArgument, &simFails[i]));
		
			if (strcmp(simFails[i],"Keyboard")==0)
			{
				if (processCommand == VTRUE)
					fails.Keyboard = 4;
				*currentArgument += 1;
			}		
			else if (strcmp(simFails[i],"Video")==0)
			{
				if (processCommand == VTRUE)
					fails.Video = 15;
				*currentArgument += 1;
			}
			else if (strcmp(simFails[i],"Power")==0)
			{
				if (processCommand == VTRUE)
					fails.Power = VTRUE;
				*currentArgument += 1;
			}
			else if (strcmp(simFails[i],"CPU")==0)
			{
				if (processCommand == VTRUE)
					fails.CPU = VTRUE;
				*currentArgument += 1;
			}
			else if (strcmp(simFails[i],"ROM")==0)
			{
				if (processCommand == VTRUE)
					fails.ROM = VTRUE;
				*currentArgument += 1;
			}	
			else if (strcmp(simFails[i],"RAM")==0)
			{
				if (processCommand == VTRUE)
					fails.RAM = VTRUE;
				*currentArgument += 1;
			}
			else
			{
				break;
			}	
		}
	}
	else
	{   
		char buf[1024];
		*errorProcessing = TSUIConst_ProcessCommandLineError_CustomError;
		CA_FreeMemory(*errorMessage);
		*errorMessage = CA_AllocMemory(sizeof(buf));
		Fmt(buf, "%s<%s%s", "Invalid command line argument: ",currentArg);
		strncpy (*errorMessage, buf, 1024);
	}
	
Error:
	free(simFails);
	DisplayError(error);
	return error < 0 ? E_FAIL : S_OK;
}

// this callback handles a specific UI message, which provides easy access to Sequence Context of the client sequence file.
HRESULT CVICALLBACK ApplicationMgr_OnUserMessage (CAObjHandle caServerObjHandle, void *caCallbackData, TSUIObj_UIMessage  uiMsg)
{
	int error = 0;
	LPUNKNOWN tempPropObject = 0;
	CAObjHandle SeqContext = 0;
	enum TSEnum_UIMessageCodes msgEvent;
	
	tsErrChk( TS_UIMessageGetEvent (uiMsg, &errorInfo, &msgEvent));
	tsErrChk( TS_UIMessageGetActiveXData (uiMsg, &errorInfo, &tempPropObject));
	CA_CreateObjHandleFromInterface (tempPropObject, &IID_IUnknown, 0, LOCALE_NEUTRAL, 0, 0, &SeqContext);
	
	if (msgEvent == TS_UIMsg_UserMessageBase +1)
	{
		tsErrChk( TS_PropertySetValBoolean 	( SeqContext, &errorInfo, "Locals.UiMsgHandled", 0, VTRUE));
		tsErrChk( TS_PropertySetValNumber 	( SeqContext, &errorInfo, "Locals.KeyboardValue", 0, fails.Keyboard));
		tsErrChk( TS_PropertySetValNumber 	( SeqContext, &errorInfo, "Locals.VideoValue", 0, fails.Video));
		tsErrChk( TS_PropertySetValBoolean 	( SeqContext, &errorInfo, "Locals.PowerFail", 0, fails.Power));
		tsErrChk( TS_PropertySetValBoolean 	( SeqContext, &errorInfo, "Locals.CPUFail", 0, fails.CPU));
		tsErrChk( TS_PropertySetValBoolean 	( SeqContext, &errorInfo, "Locals.ROMFail", 0, fails.ROM));
		tsErrChk( TS_PropertySetValBoolean 	( SeqContext, &errorInfo, "Locals.RAMFail", 0, fails.RAM));
	}
	
Error:
	
	CA_DiscardObjHandle(SeqContext);
	DisplayError(error);
	return error < 0 ? E_FAIL : S_OK;	
}

//------------------------------------------------------
//	  SETTING UP THE USER INTERFACE
//
// Register ActiveX event callbacks for TestStand events, and connect TestStand controls to give them functionality.
//------------------------------------------------------

// obtain ActiveX control handles and register ActiveX event callbacks
static int SetupActiveXControls(void)
{
	int			error = 0;
	CAObjHandle	connection = 0;

	// get handles to ActiveX controls
	errChk( GetObjHandleFromActiveXCtrl(gMainWindow.panel, MAINPANEL_APPLICATIONMGR, 		&gMainWindow.applicationMgr));
	errChk( GetObjHandleFromActiveXCtrl(gMainWindow.panel, MAINPANEL_SEQUENCEFILEVIEWMGR,	&gMainWindow.sequenceFileViewMgr));
	errChk( GetObjHandleFromActiveXCtrl(gMainWindow.panel, MAINPANEL_EXECUTIONVIEWMGR, 		&gMainWindow.executionViewMgr));

	errChk( GetObjHandleFromActiveXCtrl(gMainWindow.panel, MAINPANEL_OPENFILEBTN, 			&gMainWindow.openFileBtn));
	errChk( GetObjHandleFromActiveXCtrl(gMainWindow.panel, MAINPANEL_RUNSELECTEDBTN,		&gMainWindow.runSelectedBtn));
	errChk( GetObjHandleFromActiveXCtrl(gMainWindow.panel, MAINPANEL_CHECKBOX,				&gMainWindow.tracingEnabledCheckBox));
	errChk( GetObjHandleFromActiveXCtrl(gMainWindow.panel, MAINPANEL_SEQUENCEVIEW, 			&gMainWindow.sequenceView));
	errChk( GetObjHandleFromActiveXCtrl(gMainWindow.panel, MAINPANEL_LABEL,			 		&gMainWindow.label));
	errChk( GetObjHandleFromActiveXCtrl(gMainWindow.panel, MAINPANEL_LISTBOX, 				&gMainWindow.sequenceListBox));

	//------------------------------------------------------
	//	  REGISTERING CALLBACKS
	//
	// These calls specify callback functions which execute when various TestStand events occur.
	// For example, the ApplicationMgr_OnExitApplication callback is executed if the user attempts to close the UI.
	//------------------------------------------------------

	errChk( TSUI__ApplicationMgrEventsRegOnExitApplication(gMainWindow.applicationMgr, 					ApplicationMgr_OnExitApplication, NULL, 1, NULL));
	errChk( TSUI__ApplicationMgrEventsRegOnReportError(gMainWindow.applicationMgr, 						ApplicationMgr_OnReportError, NULL, 1, NULL));
	errChk( TSUI__ApplicationMgrEventsRegOnDisplaySequenceFile(gMainWindow.applicationMgr,				ApplicationMgr_OnDisplaySequenceFile, NULL, 1, NULL));
	errChk( TSUI__ApplicationMgrEventsRegOnDisplayExecution(gMainWindow.applicationMgr, 				ApplicationMgr_OnDisplayExecution, NULL, 1, NULL));
	errChk(	TSUI__ApplicationMgrEventsRegOnProcessUserCommandLineArguments(gMainWindow.applicationMgr, 	ApplicationMgr_OnProcessUserCommandLineArguments, NULL, 1, NULL));
	errChk( TSUI__ApplicationMgrEventsRegOnUserMessage (gMainWindow.applicationMgr,						ApplicationMgr_OnUserMessage , NULL, 1, NULL));
	
	
	//------------------------------------------------------
	//	  CONNECTING MANAGER CONTROLS TO VISIBLE CONTROLS
	//
	// The functionality of TestStand UI controls is implemented by connecting them to a manager control.
	// Each manager control provides connection methods to accomplish this. The functionality of the visible control depends on the type of connection and the connection parameters.
	// The four types of connections are demonstrated in this example.  Refer to the Connecting Manager Controls to Visible Controls topic in the TestStand help for more information
	//------------------------------------------------------

	// "Command Connections" are used to configure a TestStand Button or Checkbox control control to execute a specified command when clicked.
	// Changing the cmdKind Input will change the functionality of the button.
	tsErrChk( TSUI_SequenceFileViewMgrConnectCommand(gMainWindow.sequenceFileViewMgr, &errorInfo, gMainWindow.openFileBtn, TSUIConst_CommandKind_OpenSequenceFiles, 0, TSUIConst_CommandConnection_NoOptions, NULL));
	tsErrChk( TSUI_SequenceFileViewMgrConnectCommand(gMainWindow.sequenceFileViewMgr, &errorInfo, gMainWindow.runSelectedBtn, TSUIConst_CommandKind_RunCurrentSequence, 0, TSUIConst_CommandConnection_NoOptions, NULL));
	tsErrChk( TSUI_ApplicationMgrConnectCommand(gMainWindow.applicationMgr, &errorInfo, gMainWindow.tracingEnabledCheckBox, TSUIConst_CommandKind_TracingEnabled, 0, TSUIConst_CommandConnection_NoOptions, NULL));

	// "Information Source" Connections are used to display caption, image, and numeric information on Label controls, ExpressionEdit controls, and StatusBar panes.
	// Changing the source Input will change what information is displayed in the connected control.
	tsErrChk( TSUI_SequenceFileViewMgrConnectCaption(gMainWindow.sequenceFileViewMgr, &errorInfo, gMainWindow.label, TSUIConst_CaptionSource_CurrentSequenceFile, VFALSE, NULL));

	// "List Connections" are used to configure a TestStand ComboBox, ListBarPage, or ListBox to display a TestStand List, such as the sequences in the current file.
	// The manager controls provide a separate method for each connection type, so no cmdKind parameter is needed.
	tsErrChk( TSUI_SequenceFileViewMgrConnectSequenceList(gMainWindow.sequenceFileViewMgr, &errorInfo, gMainWindow.sequenceListBox, NULL));

	// "View Connections" are used to activate a SequenceView or ReportView control.
	// The manager controls provide a separate method for each connection type, so no cmdKind parameter is needed.
	tsErrChk( TSUI_ExecutionViewMgrConnectExecutionView(gMainWindow.executionViewMgr, &errorInfo, gMainWindow.sequenceView, TSUIConst_ExecutionViewConnection_NoOptions, NULL));

	// show all step groups at once in the sequence view
	tsErrChk( TSUI_ExecutionViewMgrSetStepGroupMode (gMainWindow.executionViewMgr, &errorInfo, TSUIConst_StepGroupMode_AllGroups));
	
	// Add custom command line arguments help
	tsErrChk( TSUI_ApplicationMgrAddCommandLineArgumentsHelp (gMainWindow.applicationMgr, &errorInfo, "[/SimulateFailure <failures>]", "<failures>: components to fail separated by spaces.\nvalid parameters are: Keyboard, Video, Power, CPU, ROM, & RAM", ""));

Error:
	CA_DiscardObjHandle(connection);
	return error;
}

//------------------------------------------------------
//	  SUPPORT FUNCTIONS
//------------------------------------------------------

// call this function to exit the program
static int ExitApplication(void)
{
	int		error = 0;
	VBOOL	canExitNow;

	// The first call to ApplicationMgrShutDown unloads files, logs out, runs unload callbacks, and finally triggers an OnApplicationCanExit event, which calls
	// this function again. When the second call ApplicationMgrShutdown returns true for canExitNow, we call QuitUserInterface, which causes the RunUserInterface call to return.
	tsErrChk( TSUI_ApplicationMgrShutdown(gMainWindow.applicationMgr, &errorInfo, &canExitNow));
	if (canExitNow)
		QuitUserInterface(0);

Error:
	DisplayError(error);
	return canExitNow ? TRUE : FALSE;
}

// call this function after you handle an error, unless you handle the error by calling DisplayError, which also calls this function
static void ClearErrorMessage(void)
{
	// clear out error message globals so that a future error that lacks an error description does not
	// unintentionally use the error description from a prior error.
	*errMsg = '\0';
	memset(&errorInfo, 0, sizeof(ERRORINFO));
}

// prints error information to the console, sets the ExitCode, and quits the app.
static void DisplayError(int errorCode)
{
	if (errorCode < 0)
	{
		printf("Error: %d  %s",errorCode,errMsg);		
		TSUI_ApplicationMgrSetExitCode (gMainWindow.applicationMgr, NULL, errorCode);

	}
}

///////////////////////////////////////////////////////////////////////////



