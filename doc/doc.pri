greaterThan(QT_MAJOR_VERSION, 4) {
    HELPGENERATOR = $$targetPath($$[QT_INSTALL_BINS]/qhelpgenerator) -platform minimal
    QDOC_BIN = $$targetPath($$[QT_INSTALL_BINS]/qdoc)
    COMPAT =
} else {
    HELPGENERATOR = $$targetPath($$[QT_INSTALL_BINS]/qhelpgenerator)
    QDOC_BIN = $$targetPath($$[QT_INSTALL_BINS]/qdoc3)
    COMPAT = -qt4
}


VERSION_TAG = $$replace(QTCREATOR_VERSION, "[-.]", )

isEmpty(LICENSE_TYPE):LICENSE_TYPE=opensource

# unset the installdir for qdoc, so we force generation
# of URLs for the links to the Qt documentation
QMAKE_DOCS_INSTALLDIR =

defineReplace(cmdEnv) {
    !equals(QMAKE_DIR_SEP, /): 1 ~= s,^(.*)$,(set \\1) &&,g
    return("$$1")
}

defineReplace(qdoc) {
    return("$$cmdEnv(SRCDIR=$$PWD OUTDIR=$$1 QTC_VERSION=$$QTCREATOR_VERSION QTC_VERSION_TAG=$$VERSION_TAG QTC_LICENSE_TYPE=$$LICENSE_TYPE QT_INSTALL_DOCS=$$[QT_INSTALL_DOCS]) $$QDOC_BIN")
}

QHP_FILE = $$OUT_PWD/doc/html/qtcreator.qhp
QCH_FILE = $$IDE_DOC_PATH/qtcreator.qch

HELP_DEP_FILES = $$PWD/src/qtcreator.qdoc \
                 $$PWD/addressbook-sdk.qdoc \
                 $$PWD/config/compat.qdocconf \
                 $$PWD/config/macros.qdocconf \
                 $$PWD/config/qt-cpp-ignore.qdocconf \
                 $$PWD/config/qt-defines.qdocconf \
                 $$PWD/config/qt-html-templates.qdocconf \
                 $$PWD/config/qt-html-default-styles.qdocconf \
                 $$PWD/qtcreator$${COMPAT}.qdocconf

html_docs.commands = $$qdoc($$OUT_PWD/doc/html) $$PWD/qtcreator$${COMPAT}.qdocconf
html_docs.depends += $$HELP_DEP_FILES
html_docs.files = $$QHP_FILE

html_docs_online.commands = $$qdoc($$OUT_PWD/doc/html) $$PWD/qtcreator-online$${COMPAT}.qdocconf
html_docs_online.depends += $$HELP_DEP_FILES

qch_docs.commands = $$HELPGENERATOR -o \"$$QCH_FILE\" $$QHP_FILE
qch_docs.depends += html_docs

DEV_QHP_FILE = $$OUT_PWD/doc/html-dev/qtcreator-dev.qhp
DEV_QCH_FILE = $$IDE_DOC_PATH/qtcreator-dev.qch

DEV_HELP_DEP_FILES = \
    $$PWD/api/qtcreator-api.qdoc \
    $$PWD/api/coding-style.qdoc \
    $$PWD/api/external-tool-spec.qdoc \
    $$PWD/api/qtcreator-dev.qdoc \
    $$PWD/api/qtcreator-dev-wizards.qdoc \
    $$PWD/api/creating-plugins.qdoc \
    $$PWD/api/getting-and-building.qdoc \
    $$PWD/api/first-plugin.qdoc \
    $$PWD/api/plugin-specifications.qdoc \
    $$PWD/api/plugin-lifecycle.qdoc \
    $$PWD/api/pluginmanager.qdoc \
    $$PWD/api/qtcreator-dev$${COMPAT}.qdocconf

dev_html_docs.commands = $$qdoc($$OUT_PWD/doc/html-dev) $$PWD/api/qtcreator-dev$${COMPAT}.qdocconf
dev_html_docs.depends += $$DEV_HELP_DEP_FILES

dev_html_docs_online.commands = $$qdoc($$OUT_PWD/doc/html-dev) $$PWD/api/qtcreator-dev-online$${COMPAT}.qdocconf
dev_html_docs_online.depends += $$DEV_HELP_DEP_FILES

dev_qch_docs.commands = $$HELPGENERATOR -o \"$$DEV_QCH_FILE\" $$DEV_QHP_FILE
dev_qch_docs.depends += dev_html_docs

!macx {
    inst_qch_docs.files = $$QCH_FILE
    inst_qch_docs.path = $$QTC_PREFIX/share/doc/qtcreator
    inst_qch_docs.CONFIG += no_check_exist no_default_install
    INSTALLS += inst_qch_docs

    inst_dev_qch_docs.files = $$DEV_QCH_FILE
    inst_dev_qch_docs.path = $$QTC_PREFIX/share/doc/qtcreator
    inst_dev_qch_docs.CONFIG += no_check_exist no_default_install
    INSTALLS += inst_dev_qch_docs

    install_docs.depends = install_inst_qch_docs install_inst_dev_qch_docs
    QMAKE_EXTRA_TARGETS += install_docs
}

docs_online.depends = html_docs_online dev_html_docs_online
docs.depends = qch_docs dev_qch_docs
QMAKE_EXTRA_TARGETS += html_docs dev_html_docs html_docs_online dev_html_docs_online qch_docs dev_qch_docs docs docs_online

OTHER_FILES = $$HELP_DEP_FILES $$DEV_HELP_DEP_FILES

fixnavi.commands = \
    cd $$targetPath($$PWD) && \
    perl fixnavi.pl -Dqcmanual -Dqtquick src
QMAKE_EXTRA_TARGETS += fixnavi
