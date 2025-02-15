﻿#include "Engine/Core/CommandSystem.hpp"
#include "Engine/Core/ErrorUtils.hpp"
#include "Engine/Core/Console.hpp"

CommandSystem* CommandSystem::s_defaultCommandSystem;

CommandParameterParser::CommandParameterParser(
    const String& commandString, bool suppressWarnings )
    :m_suppressWarnings( suppressWarnings )
{
    StringUtils::ParseFunctionName( commandString, m_commandName );

    ParseStatus status = StringUtils::ParseParameters(
        commandString, m_parameterTokens );
    if( PARSE_SUCCESS != status )
    {
        if( !m_suppressWarnings )
            LOG_INVALID_PARAMETERS( m_commandName );
        m_allParseSuccess = false;
    }
}

CommandSystem* CommandSystem::DefaultCommandSystem()
{
    if( s_defaultCommandSystem == nullptr )
        s_defaultCommandSystem = new CommandSystem();

    return s_defaultCommandSystem;
}

void CommandSystem::AddCommand( String name, CommandCallback callback )
{
    if( m_commandDefs.find( name ) != m_commandDefs.end() )
    {
        LOG_WARNING( "command already exists: " + name );
        return;
    }
    m_commandDefs[name] = CommandDef( name, callback );
}

void CommandSystem::RunCommand( String commandString )
{
    String commandName;
    StringUtils::ParseFunctionName( commandString, commandName );
    if( m_commandDefs.find( commandName ) == m_commandDefs.end() )
    {
        Console::DefaultConsole()->Print( "INVALID COMMAND: " + commandName, Rgba::RED );
        return;
    }
    m_commandDefs[commandName].m_callback( commandString );
}


CommandSelfRegister::CommandSelfRegister( String name, CommandCallback callback )
{
    CommandSystem::DefaultCommandSystem()->AddCommand( name, callback );
}
