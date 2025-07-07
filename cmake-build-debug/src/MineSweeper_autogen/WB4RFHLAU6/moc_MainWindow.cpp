/****************************************************************************
** Meta object code from reading C++ file 'MainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/View/MainWindow.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MainWindow.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "newGameRequested",
        "",
        "GameMode",
        "mode",
        "GameDifficulty",
        "difficulty",
        "nextCampaignLevelRequested",
        "returnToHomeRequested",
        "hintRequested",
        "revealCellRequested",
        "row",
        "col",
        "toggleFlagRequested",
        "cycleMarkRequested",
        "onBoardSizeChanged",
        "newSize",
        "onCellUpdated",
        "CellUpdateInfo",
        "info",
        "onShowGameOverDialog",
        "message",
        "isCampaign",
        "isVictory",
        "isLastLevel",
        "updateFlagsLabel",
        "flags",
        "updateStatusLabel",
        "text",
        "showTemporaryMessage",
        "durationMs",
        "configureForMode",
        "currentLevel",
        "totalLevels",
        "updateTimerDisplay",
        "seconds",
        "setHintButtonEnabled",
        "enabled",
        "on_newGameButton_clicked",
        "on_homeButton_clicked",
        "on_hintButton_clicked"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'newGameRequested'
        QtMocHelpers::SignalData<void(GameMode, GameDifficulty)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 5, 6 },
        }}),
        // Signal 'nextCampaignLevelRequested'
        QtMocHelpers::SignalData<void()>(7, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'returnToHomeRequested'
        QtMocHelpers::SignalData<void()>(8, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'hintRequested'
        QtMocHelpers::SignalData<void()>(9, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'revealCellRequested'
        QtMocHelpers::SignalData<void(int, int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Int, 12 },
        }}),
        // Signal 'toggleFlagRequested'
        QtMocHelpers::SignalData<void(int, int)>(13, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Int, 12 },
        }}),
        // Signal 'cycleMarkRequested'
        QtMocHelpers::SignalData<void(int, int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 }, { QMetaType::Int, 12 },
        }}),
        // Slot 'onBoardSizeChanged'
        QtMocHelpers::SlotData<void(const QSize &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QSize, 16 },
        }}),
        // Slot 'onCellUpdated'
        QtMocHelpers::SlotData<void(const CellUpdateInfo &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Slot 'onShowGameOverDialog'
        QtMocHelpers::SlotData<void(const QString &, bool, bool, bool)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 21 }, { QMetaType::Bool, 22 }, { QMetaType::Bool, 23 }, { QMetaType::Bool, 24 },
        }}),
        // Slot 'updateFlagsLabel'
        QtMocHelpers::SlotData<void(int)>(25, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 26 },
        }}),
        // Slot 'updateStatusLabel'
        QtMocHelpers::SlotData<void(const QString &)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 28 },
        }}),
        // Slot 'showTemporaryMessage'
        QtMocHelpers::SlotData<void(const QString &, int)>(29, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 21 }, { QMetaType::Int, 30 },
        }}),
        // Slot 'configureForMode'
        QtMocHelpers::SlotData<void(GameMode, const GameDifficulty &, int, int)>(31, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 3, 4 }, { 0x80000000 | 5, 6 }, { QMetaType::Int, 32 }, { QMetaType::Int, 33 },
        }}),
        // Slot 'updateTimerDisplay'
        QtMocHelpers::SlotData<void(int)>(34, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 35 },
        }}),
        // Slot 'setHintButtonEnabled'
        QtMocHelpers::SlotData<void(bool)>(36, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 37 },
        }}),
        // Slot 'on_newGameButton_clicked'
        QtMocHelpers::SlotData<void()>(38, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_homeButton_clicked'
        QtMocHelpers::SlotData<void()>(39, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'on_hintButton_clicked'
        QtMocHelpers::SlotData<void()>(40, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->newGameRequested((*reinterpret_cast< std::add_pointer_t<GameMode>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<GameDifficulty>>(_a[2]))); break;
        case 1: _t->nextCampaignLevelRequested(); break;
        case 2: _t->returnToHomeRequested(); break;
        case 3: _t->hintRequested(); break;
        case 4: _t->revealCellRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 5: _t->toggleFlagRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->cycleMarkRequested((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->onBoardSizeChanged((*reinterpret_cast< std::add_pointer_t<QSize>>(_a[1]))); break;
        case 8: _t->onCellUpdated((*reinterpret_cast< std::add_pointer_t<CellUpdateInfo>>(_a[1]))); break;
        case 9: _t->onShowGameOverDialog((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[4]))); break;
        case 10: _t->updateFlagsLabel((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 11: _t->updateStatusLabel((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 12: _t->showTemporaryMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 13: _t->configureForMode((*reinterpret_cast< std::add_pointer_t<GameMode>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<GameDifficulty>>(_a[2])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[4]))); break;
        case 14: _t->updateTimerDisplay((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 15: _t->setHintButtonEnabled((*reinterpret_cast< std::add_pointer_t<bool>>(_a[1]))); break;
        case 16: _t->on_newGameButton_clicked(); break;
        case 17: _t->on_homeButton_clicked(); break;
        case 18: _t->on_hintButton_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)(GameMode , GameDifficulty )>(_a, &MainWindow::newGameRequested, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)()>(_a, &MainWindow::nextCampaignLevelRequested, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)()>(_a, &MainWindow::returnToHomeRequested, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)()>(_a, &MainWindow::hintRequested, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)(int , int )>(_a, &MainWindow::revealCellRequested, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)(int , int )>(_a, &MainWindow::toggleFlagRequested, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (MainWindow::*)(int , int )>(_a, &MainWindow::cycleMarkRequested, 6))
            return;
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 19)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 19;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 19)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 19;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::newGameRequested(GameMode _t1, GameDifficulty _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void MainWindow::nextCampaignLevelRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void MainWindow::returnToHomeRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void MainWindow::hintRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void MainWindow::revealCellRequested(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2);
}

// SIGNAL 5
void MainWindow::toggleFlagRequested(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1, _t2);
}

// SIGNAL 6
void MainWindow::cycleMarkRequested(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}
QT_WARNING_POP
