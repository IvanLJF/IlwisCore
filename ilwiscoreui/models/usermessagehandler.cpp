#include "kernel.h"
#include "usermessagehandler.h"

using namespace Ilwis;

UserMessageHandler::UserMessageHandler(QObject *parent) :
    QObject(parent)
{
    QList<IssueObject> issues;
    kernel()->issues()->copy(issues);
    for(auto issue : issues)
        if ( issue.type() == IssueObject::itRESULT)
            _results.insert(0, new MessageModel(issue, this));
        else if (issue.type() == IssueObject::itExpression ){
            _commands.insert(0, new MessageModel(issue, this));
        }else
            _messages.insert(0, new MessageModel(issue, this));
}

QMLMessageList UserMessageHandler::messages()
{
    return  QQmlListProperty<MessageModel>(this, _messages);
}

QMLMessageList UserMessageHandler::commands()
{
    return QQmlListProperty<MessageModel>(this, _commands);
}

QString UserMessageHandler::results()
{
    QString all;
   for(int i =0; i < _results.size(); ++i){
       if ( all != "")
           all += "\n";
        all += _results[i]->message();
   }
   return all;
}

void UserMessageHandler::addMessage(const IssueObject& issue)
{
    if ( issue.type() == IssueObject::itRESULT){
        _results.insert(0, new MessageModel(issue, this));
        emit resultsChanged();
    }
    else  if (issue.type() == IssueObject::itExpression ){
        _commands.insert(0, new MessageModel(issue, this));
        emit commandsChanged();
    }else {
        _messages.insert(0, new MessageModel(issue, this));
        emit messageChanged();
    }

    emit messageIconChanged();
}

QString UserMessageHandler::messageIcon() const{
    if ( _messages.size() > 0){
        return _messages.first()->messageIcon();
    }
    return "message40.png";
}

void UserMessageHandler::resetColor(int index)
{
    if ( index < _messages.size()){
        if ( index < 0){
            for(int i = 0; i < _messages.size(); ++i){
                _messages[i]->resetColor();
            }
        }else
            _messages[index]->resetColor();
        emit messageChanged();
        emit messageIconChanged();
    }
}

void UserMessageHandler::clearResults()
{
    _results.clear();
   emit resultsChanged();
   emit messageIconChanged();
}

//---------------------------------------------------
MessageModel::MessageModel()
{

}

MessageModel::MessageModel(const IssueObject &issue, QObject *parent) : QObject(parent), _message(issue)
{

}

QString MessageModel::time() const
{
    return  Ilwis::Time(_message.time()).toString();
}

QString MessageModel::shorttime() const
{
    return Ilwis::Time(_message.time()).toString(false, itTIME);
}

QString MessageModel::type() const
{
    switch(_message.type()){
    case IssueObject::itWarning:
        return "warning";
    case IssueObject::itError:
        return "error";
    case IssueObject::itCritical:
        return "critical error";
    case IssueObject::itMessage:
        return "notification";
    case IssueObject::itDebug:
        return "trace";
    case IssueObject::itExpression:
        return "command";
    default:
        return "";
    }
}

QString MessageModel::message() const
{
    return _message.message();
}

quint64 MessageModel::id()
{
    return _message.id();
}

QColor MessageModel::color() const
{
    if ( _isReset)
        return "green";

    switch(_message.type()){
    case IssueObject::itWarning:
        return QColor("orange");
    case IssueObject::itError:
        return QColor("red");
    case IssueObject::itCritical:
        return QColor("purple");
    case IssueObject::itMessage:
        return "green";
    case IssueObject::itDebug:
        return "blue";
    case IssueObject::itRESULT:
        return "black";
    case IssueObject::itExpression:
        return "black";
    default:
        return "";
    }
}

QString MessageModel::messageIcon() const
{
    if ( _isReset)
        return "message.png";
    switch(_message.type()){
    case IssueObject::itWarning:
        return "message_large_warning.png";
    case IssueObject::itError:
        return "message_large_critical.png";
    case IssueObject::itCritical:
        return "message_large_critical.png";
    case IssueObject::itRESULT:
        return "message_large_result.png";
    default:
        return "message.png";

    }
}

void MessageModel::resetColor()
{
    _isReset = true;
}
