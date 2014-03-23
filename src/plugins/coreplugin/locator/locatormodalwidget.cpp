#include "locatormodalwidget.h"
#include "locatorwidget.h"
#include <QVBoxLayout>

namespace Core {
namespace Internal {

LocatorModalWidget::LocatorModalWidget(LocatorWidget *locatorWidget)
{
    setWindowModality(Qt::WindowModal);


    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(locatorWidget);
    setLayout(layout);
}

}
}
