/****************************************************************************
** Meta object code from reading C++ file 'GameViewModel.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/ViewModel/GameViewModel.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GameViewModel.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN13GameViewModelE_t {};
} // unnamed namespace

template <> constexpr inline auto GameViewModel::qt_create_metaobjectdata<qt_meta_tag_ZN13GameViewModelE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "GameViewModel",
        "highScoresReady",
        "",
        "QList<HighScoreItem>",
        "scores",
        "boardSizeChanged",
        "newSize",
        "cellUpdated",
        "CellUpdateInfo",
        "info",
        "showGameOverDialog",
        "message",
        "isCampaign",
        "isVictory",
        "isLastLevel",
        "flagsLabelUpdated",
        "flags",
        "statusLabelUpdated",
        "text",
        "temporaryMessageShown",
        "durationMs",
        "modeConfigured",
        "GameMode",
        "mode",
        "GameDifficulty",
        "difficulty",
        "currentLevel",
        "totalLevels",
        "timerDisplayUpdated",
        "seconds",
        "hintButtonEnabled",
        "enabled",
        "returnToHomeRequested",
        "startNewGame",
        "startNextCampaignLevel",
        "revealCellRequest",
        "row",
        "col",
        "toggleFlagRequest",
        "cycleMarkRequest",
        "hintRequest",
        "loadHighScoresRequest",
        "clearHighScoresRequest",
        "onRequestReturnToHome",
        "onModelChanged",
        "onGameOver",
        "victory",
        "onTimerTick"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'highScoresReady'
        QtMocHelpers::SignalData<void(const QVector<HighScoreItem> &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 },
        }}),
        // Signal 'boardSizeChanged'
        QtMocHelpers::SignalData<void(const QSize &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QSize, 6 },
        }}),
        // Signal 'cellUpdated'
        QtMocHelpers::SignalData<void(const CellUpdateInfo &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Signal 'showGameOverDialog'
        QtMocHelpers::SignalData<void(const QString &, bool, bool, bool)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 }, { QMetaType::Bool, 12 }, { QMetaType::Bool, 13 }, { QMetaType::Bool, 14 },
        }}),
        // Signal 'flagsLabelUpdated'
        QtMocHelpers::SignalData<void(int)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 16 },
        }}),
        // Signal 'statusLabelUpdated'
        QtMocHelpers::SignalData<void(const QString &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 18 },
        }}),
        // Signal 'temporaryMessageShown'
        QtMocHelpers::SignalData<void(const QString &, int)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 }, { QMetaType::Int, 20 },
        }}),
        // Signal 'modeConfigured'
        QtMocHelpers::SignalData<void(GameMode, const GameDifficulty &, int, int)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 22, 23 }, { 0x80000000 | 24, 25 }, { QMetaType::Int, 26 }, { QMetaType::Int, 27 },
        }}),
        // Signal 'timerDisplayUpdated'
        QtMocHelpers::SignalData<void(int)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 29 },
        }}),
        // Signal 'hintButtonEnabled'
        QtMocHelpers::SignalData<void(bool)>(30, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 31 },
        }}),
        // Signal 'returnToHomeRequested'
        QtMocHelpers::SignalData<void()>(32, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'startNewGame'
        QtMocHelpers::SlotData<void(GameMode, GameDifficulty)>(33, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 22, 23 }, { 0x80000000 | 24, 25 },
        }}),
        // Slot 'startNextCampaignLevel'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'revealCellRequest'
        QtMocHelpers::SlotData<void(int, int)>(35, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 36 }, { QMetaType::Int, 37 },
        }}),
        // Slot 'toggleFlagRequest'
        QtMocHelpers::SlotData<void(int, int)>(38, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 36 }, { QMetaType::Int, 37 },
        }}),
        // Slot 'cycleMarkRequest'
        QtMocHelpers::SlotData<void(int, int)>(39, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 36 }, { QMetaType::Int, 37 },
        }}),
        // Slot 'hintRequest'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'loadHighScoresRequest'
        QtMocHelpers::SlotData<void()>(41, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'clearHighScoresRequest'
        QtMocHelpers::SlotData<void()>(42, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onRequestReturnToHome'
        QtMocHelpers::SlotData<void()>(43, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onModelChanged'
        QtMocHelpers::SlotData<void()>(44, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onGameOver'
        QtMocHelpers::SlotData<void(bool)>(45, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 46 },
        }}),
        // Slot 'onTimerTick'
        QtMocHelpers::SlotData<void()>(47, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<GameViewModel, qt_meta_tag_ZN13GameViewModelE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject GameViewModel::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13GameViewModelE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13GameViewModelE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN13GameViewModelE_t>.metaTypes,
    nullptr
} };

void GameViewModel::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<GameViewModel *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->highScoresReady((*reinterpret_cast< std::add_pointer_t<QList<HighScoreItem>>>(_a[1]))); break;
        case 1: _t->boardSizeChanged((*reinterpret_cast< std::add_pointer_t<QSize>>(_a[1]))); break;
        case 2: _t->cellUpdated((*reinterpret_cast< std::add_pointer_t<CellUpdateInfo>>(_a[1]))); break;
        case 3: _t->showGameOverDialog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[4]))); break;
        case 4: _t->flagsLabelUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->statusLabelUpdated((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->temporaryMessageShown((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->modeConfigured((*reinterpret_cast< std::add_pointer_t<GameMode>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<GameDifficulty>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4]))); break;
        case 8: _t->timerDisplayUpdated((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 9: _t->hintButtonEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 10: _t->returnToHomeRequested(); break;
        case 11: _t->startNewGame((*reinterpret_cast< std::add_pointer_t<GameMode>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<GameDifficulty>>(_a[2]))); break;
        case 12: _t->startNextCampaignLevel(); break;
        case 13: _t->revealCellRequest((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 14: _t->toggleFlagRequest((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 15: _t->cycleMarkRequest((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 16: _t->hintRequest(); break;
        case 17: _t->loadHighScoresRequest(); break;
        case 18: _t->clearHighScoresRequest(); break;
        case 19: _t->onRequestReturnToHome(); break;
        case 20: _t->onModelChanged(); break;
        case 21: _t->onGameOver((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 22: _t->onTimerTick(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (GameViewModel::*)(const QVector<HighScoreItem> & )>(_a, &GameViewModel::highScoresReady, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameViewModel::*)(const QSize & )>(_a, &GameViewModel::boardSizeChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameViewModel::*)(const CellUpdateInfo & )>(_a, &GameViewModel::cellUpdated, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameViewModel::*)(const QString & , bool , bool , bool )>(_a, &GameViewModel::showGameOverDialog, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameViewModel::*)(int )>(_a, &GameViewModel::flagsLabelUpdated, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameViewModel::*)(const QString & )>(_a, &GameViewModel::statusLabelUpdated, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameViewModel::*)(const QString & , int )>(_a, &GameViewModel::temporaryMessageShown, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameViewModel::*)(GameMode , const GameDifficulty & , int , int )>(_a, &GameViewModel::modeConfigured, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameViewModel::*)(int )>(_a, &GameViewModel::timerDisplayUpdated, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameViewModel::*)(bool )>(_a, &GameViewModel::hintButtonEnabled, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (GameViewModel::*)()>(_a, &GameViewModel::returnToHomeRequested, 10))
            return;
    }
}

const QMetaObject *GameViewModel::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GameViewModel::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN13GameViewModelE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int GameViewModel::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void GameViewModel::highScoresReady(const QVector<HighScoreItem> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void GameViewModel::boardSizeChanged(const QSize & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void GameViewModel::cellUpdated(const CellUpdateInfo & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void GameViewModel::showGameOverDialog(const QString & _t1, bool _t2, bool _t3, bool _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 4
void GameViewModel::flagsLabelUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void GameViewModel::statusLabelUpdated(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void GameViewModel::temporaryMessageShown(const QString & _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}

// SIGNAL 7
void GameViewModel::modeConfigured(GameMode _t1, const GameDifficulty & _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 8
void GameViewModel::timerDisplayUpdated(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void GameViewModel::hintButtonEnabled(bool _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1);
}

// SIGNAL 10
void GameViewModel::returnToHomeRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 10, nullptr);
}
QT_WARNING_POP
