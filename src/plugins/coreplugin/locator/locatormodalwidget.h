#ifndef LOCATORMODALWIDGET_H
#define LOCATORMODALWIDGET_H

#include <QWidget>

namespace Core {
namespace Internal {

class LocatorWidget;

class LocatorModalWidget : public QWidget
{
public:
    LocatorModalWidget(LocatorWidget* locatorWidget);
};

}
}

#endif // LOCATORMODALWIDGET_H
