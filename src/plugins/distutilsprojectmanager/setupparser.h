/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010-2011 Openismus GmbH.
**   Authors: Peter Penz (ppenz@openismus.com)
**            Patricia Santana Cruz (patriciasantanacruz@gmail.com)
**
** Contact: Nokia Corporation (info@qt.nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at info@qt.nokia.com.
**
**************************************************************************/

#ifndef MAKEFILEPARSER_H
#define MAKEFILEPARSER_H

#include <QMutex>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QObject>
#include <QDir>
#include <QFileInfo>

namespace DistutilsProjectManager {
namespace Internal {

/**
 * @brief Parses the distutils makefile Makefile.am.
 *
 * The parser returns the sources, makefiles and executable.
 * Variables like $(test) are not evaluated. If such a variable
 * is part of a SOURCES target, a fallback will be done and all
 * sub directories get parsed for C- and C++ files.
 */
class SetupParser : public QObject
{
    Q_OBJECT

public:
    /**
     * @param makefile  Filename including path of the distutils
     *                  makefile that should be parsed.
     */
    SetupParser(const QString &projectFilePath);

    /**
     * Parses the project file.
     * Must be invoked at least once, otherwise
     * the getter methods of MakefileParser will return empty values.
     * @return True, if the parsing was successful. If false is returned,
     *         the makefile could not be opened.
     */
    bool parse();

    /**
     * Cancels the parsing. Calling this method only makes sense, if the
     * parser runs in a different thread than the caller of this method.
     * The method is thread-safe.
     */
    void cancel();

    /**
     * @return True, if the parser has been cancelled by MakefileParser::cancel().
     *         The method is thread-safe.
     */
    bool isCanceled() const;

    QStringList sourceFiles() const;

    /**
     * @return
     */
    QStringList projectFiles() const;

    /**
     * @return
     */
    QMap<QString, QStringList> packages() const;

    /**
     * @return File names of the scripts.
     */
    QStringList scripts() const;

signals:
    /**
     * Is emitted periodically during parsing the Makefile.am files
     * and the sub directories. \p status provides a translated
     * string, that can be shown to indicate the current state
     * of the parsing.
     */
    void status(const QString &status);

private:
    bool parseHelperOutput(QByteArray output);
    bool parseManifestTemplate(const QString &manifestTemplatePath);
    bool addSourceFileIfExisting(const QString& fileName);

private:
    bool m_success;             ///< Return value for MakefileParser::parse().
    bool m_cancel;              ///< True, if the parsing should be cancelled.
    mutable QMutex m_mutex;     ///< Mutex to protect m_cancel.

    QDir m_projectDirectory;
    QString m_setupFilePath;         ///< Filename of the distutils setup file
    QStringList m_scripts;       ///< Return value for MakefileParser::scripts()
    QStringList m_sourceFiles;      ///< Return value for MakefileParser::sourceFiles()
    QMap<QString, QStringList> m_packages;    ///< Return value for MakefileParser::packages()
    QStringList m_projectFiles; ///< Return value for MakefileParser::m_projectFiles()
};

} // namespace Internal
} // namespace DistutilsProjectManager

#endif // MAKEFILEPARSER_H

