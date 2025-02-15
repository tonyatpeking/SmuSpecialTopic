#pragma once

// ErrorWarningAssert.hpp
//
// Summary of error and assertion macros:
//	#define ERROR_AND_DIE( errorText )						// "MUST not reach this point"; Show error dialogue, then shut down the app
//	#define ERROR_RECOVERABLE( errorText )					// "SHOULD not reach this point"; Show warning dialogue, then proceed
//	#define GUARANTEE_OR_DIE( condition, errorText )		// "MUST be true"; If condition is false, show error dialogue then shut down the app
//	#define GUARANTEE_RECOVERABLE( condition, errorText )	// "SHOULD be true"; If condition is false, show warning dialogue then proceed
//	#define ASSERT_OR_DIE( condition, errorText )			// Same as GUARANTEE_OR_DIE, but removed if DISABLE_ASSERTS is #defined
//	#define ASSERT_RECOVERABLE( condition, errorText )		// Same as GUARANTEE_RECOVERABLE, but removed if DISABLE_ASSERTS is #defined
//


#include "Engine/Core/Types.hpp"

class Rgba;


enum SeverityLevel
{
    SEVERITY_INFORMATION,
    SEVERITY_QUESTION,
    SEVERITY_WARNING,
    SEVERITY_FATAL
};



void DebuggerPrintf( const char* messageFormat, ... );

bool IsDebuggerAvailable();
__declspec( noreturn ) void FatalError(
    const char* filePath, const char* functionName, int lineNum,
    const String& reasonForError, const char* conditionText=nullptr );

void RecoverableWarning(
    const char* filePath, const char* functionName, int lineNum,
    const String& reasonForWarning, const char* conditionText=nullptr );

void SystemDialogue_Okay(
    const String& messageTitle, const String& messageText, SeverityLevel severity );

bool SystemDialogue_OkayCancel(
    const String& messageTitle, const String& messageText, SeverityLevel severity );

bool SystemDialogue_YesNo(
    const String& messageTitle, const String& messageText, SeverityLevel severity );

int SystemDialogue_YesNoCancel(
    const String& messageTitle, const String& messageText, SeverityLevel severity );


void Log(
    const char* filePath, const char* functionName, int lineNum,
    const String& messageText, SeverityLevel severity );

Rgba SeverityLevelToColor( SeverityLevel severity );
String SeverityToString( SeverityLevel severity );

// ERROR_AND_DIE
//
// Present in all builds.
// No condition; always triggers if reached.
// Depending on the platform, this typically:
//	- Logs an error message to the console and/or log file
//	- Opens an error/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Shuts down the app
//
// Use this when reaching a certain line of code should never happen ever,
// and continued execution is dangerous or impossible.
//
#define ERROR_AND_DIE( errorMessageText )												\
{																						\
	FatalError( __FILE__,  __FUNCTION__, __LINE__, errorMessageText );					\
}



// ERROR_RECOVERABLE
//
// Present in all builds.
// No condition; always triggers if reached.
// Depending on the platform, this typically:
//	- Logs a warning message to the console and/or log file
//	- Opens an warning/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Continues execution
//
#define ERROR_RECOVERABLE( errorMessageText )											\
{																						\
	RecoverableWarning( __FILE__,  __FUNCTION__, __LINE__, errorMessageText );			\
}



// GUARANTEE_OR_DIE
//
// Present in all builds.
// Triggers if condition is false.
// Depending on the platform, this typically:
//	- Logs an error message to the console and/or log file
//	- Opens an error/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Shuts down the app
//
#define GUARANTEE_OR_DIE( condition, errorMessageText )									\
{																						\
	if( !(condition) )																	\
	{																					\
		const char* text = #condition;											        \
		FatalError( __FILE__,  __FUNCTION__, __LINE__, errorMessageText, text );        \
	}																					\
}



// GUARANTEE_RECOVERABLE
//
// Present in all builds.
// Triggers if condition is false.
// Depending on the platform, this typically:
//	- Logs a warning message to the console and/or log file
//	- Opens an warning/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Continues execution
//
#define GUARANTEE_RECOVERABLE( condition, errorMessageText )							\
{																						\
	if( !(condition) )																	\
	{																					\
		const char* text = #condition;													\
		RecoverableWarning( __FILE__,  __FUNCTION__, __LINE__, errorMessageText, text );\
	}																					\
}



// ASSERT_OR_DIE
//
// Removed if DISABLE_ASSERTS is defined, typically in a Final build configuration.
// Triggers if condition is false.
// Depending on the platform, this typically:
//	- Logs an error message to the console and/or log file
//	- Opens an error/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Shuts down the app
//
#if defined( DISABLE_ASSERTS )
#define ASSERT_OR_DIE( condition, errorMessageText ) { (void)( condition ); }
#else
#define ASSERT_OR_DIE( condition, errorMessageText )									\
{																						\
	if( !(condition) )																	\
	{																					\
		const char* text = #condition;													\
		FatalError( __FILE__,  __FUNCTION__, __LINE__, errorMessageText, text );		\
	}																				    \
}
#endif



// ASSERT_RECOVERABLE
//
// Removed if DISABLE_ASSERTS is defined, typically in a Final build configuration.
// Triggers if condition is false.
// Depending on the platform, this typically:
//	- Logs a warning message to the console and/or log file
//	- Opens an warning/message dialogue box
//	- Triggers a debug breakpoint (if appropriate development suite is present)
//	- Continues execution
//
#if defined( DISABLE_ASSERTS )
#define ASSERT_RECOVERABLE( condition, errorMessageText ) { (void)( condition ); }
#else
#define ASSERT_RECOVERABLE( condition, errorMessageText )								\
{																						\
	if( !(condition) )																	\
	{																					\
		const char* text = #condition;													\
		RecoverableWarning( __FILE__,  __FUNCTION__, __LINE__, errorMessageText, text );\
	}																					\
}
#endif



#if defined( DISABLE_LOGGING )
#define LOG(  errorMessageText ) { (void)( errorMessageText ); }
#else
#define LOG( errorMessageText )											                \
{																						\
	Log(__FILE__,  __FUNCTION__, __LINE__, errorMessageText, SEVERITY_INFORMATION );	\
}
#endif

#if defined( DISABLE_LOGGING )
#define LOG_QUESTION(  errorMessageText ) { (void)( errorMessageText ); }
#else
#define LOG_QUESTION( errorMessageText )											    \
{																						\
	Log(__FILE__,  __FUNCTION__, __LINE__, errorMessageText, SEVERITY_QUESTION );       \
}
#endif

#if defined( DISABLE_LOGGING )
#define LOG_WARNING(  errorMessageText ) { (void)( errorMessageText ); }
#else
#define LOG_WARNING( errorMessageText )                                                 \
{																			            \
	Log(__FILE__,  __FUNCTION__, __LINE__, errorMessageText, SEVERITY_WARNING );        \
}
#endif

#if defined( DISABLE_LOGGING )
#define LOG_FATAL(  errorMessageText ) { (void)( errorMessageText ); }
#else
#define LOG_FATAL( errorMessageText )											        \
{																						\
	Log(__FILE__,  __FUNCTION__, __LINE__, errorMessageText, SEVERITY_FATAL );			\
}
#endif

#if defined( DISABLE_LOGGING )
#define LOG_ASSET_LOAD_FAILED(  errorMessageText ) { (void)( errorMessageText ); }
#else
#define LOG_ASSET_LOAD_FAILED( errorMessageText )							            \
{													                                    \
	LOG_WARNING( "Asset failed to load:  [" + errorMessageText + "]  ");	            \
}
#endif

#if defined( DISABLE_LOGGING )
#define LOG_MISSING_ASSET(  errorMessageText ) { (void)( errorMessageText ); }
#else
#define LOG_MISSING_ASSET( errorMessageText )											\
{																						\
	LOG_WARNING( "Using unloaded asset:  [" + String(errorMessageText) + "]  ");	    \
}
#endif

#if defined( DISABLE_LOGGING )
#define LOG_NULL_POINTER(  errorMessageText ) { (void)( errorMessageText ); }
#else
#define LOG_NULL_POINTER( errorMessageText )										    \
{																						\
	LOG_WARNING( "Using null pointer  :  [" + String(errorMessageText) + "]  ");	    \
}
#endif

#if defined( DISABLE_LOGGING )
#define LOG_INVALID_PARAMETERS(  errorMessageText ) { (void)( errorMessageText ); }
#else
#define LOG_INVALID_PARAMETERS( errorMessageText )										\
{																						\
	LOG_WARNING( "Invalid parameters on:  [" + String(errorMessageText) + "]  ");	    \
}
#endif


// "TODO" Macro
// Source from http://www.flipcode.com/archives/FIXME_TODO_Notes_As_Warnings_In_Compiler_Output.shtml
#define _QUOTE(x) # x
#define QUOTE(x) _QUOTE(x)
#define __FILE__LINE__ __FILE__ "(" QUOTE(__LINE__) ") : "

#define PRAGMA(p)  __pragma( p )
#define NOTE( x )  PRAGMA( message(x) )
#define FILE_LINE  NOTE( __FILE__LINE__ )

// THE IMPORANT BITS
#define TODO( x )  NOTE( __FILE__LINE__"\n"           \
        " ------------------------------------------------------------------\n" \
        "|  TODO :   " ##x "\n" \
        " ------------------------------------------------------------------\n" )

// "UNIMPLEMENTED" Macro
#define UNIMPLEMENTED()  TODO( "IMPLEMENT: " QUOTE(__FILE__) " (" QUOTE(__LINE__) ")" );\
ERROR_RECOVERABLE("UNIMPLEMENTED")