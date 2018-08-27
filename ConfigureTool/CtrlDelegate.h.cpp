/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

/*
    delegate.cpp

    A delegate that allows the user to change integer values from the model
    using a spin box widget.
*/

#include "CtrlDelegate.h"
#include <QDebug>
#include <QSpinBox>
#include <QLineEdit>

//! [0]
CtrlDelegate::CtrlDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}
//! [0]
//!
void CtrlDelegate::setCtrlType(CtrlType ctrlType){
    m_ctrlType = ctrlType;
}

//! [1]
QWidget *CtrlDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex & index ) const
{
    QWidget* pEditor = NULL;
    if( SpinBox == m_ctrlType )
    {
        QSpinBox* pSpinBox = new QSpinBox(parent);
        pSpinBox->setFrame(true);
        pSpinBox->setMinimum(0);
        pSpinBox->setMaximum(100);
        pEditor = pSpinBox;
    }

    if( LineEdit == m_ctrlType )
    {
        QLineEdit* pLineEdit = new QLineEdit(parent);
        if( 0 != index.column() )
        {
            QRegExp regx("[1-9][0-9]+$");
            QValidator *validator = new QRegExpValidator(regx, pLineEdit );
            pLineEdit->setValidator(validator);
        }
        pLineEdit->setFrame(true);

        pEditor = pLineEdit;
    }
    return pEditor;
}
//! [1]

//! [2]
void CtrlDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    if( SpinBox == m_ctrlType )
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox* pSpinBox = static_cast<QSpinBox*>(editor);
        pSpinBox->setValue(value);
    }

    if( LineEdit == m_ctrlType )
    {
        QLineEdit* pLineEdit = static_cast<QLineEdit*>(editor);
        pLineEdit->setText(index.model()->data(index, Qt::EditRole).toString());
    }
}
//! [2]

//! [3]
void CtrlDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    if( SpinBox == m_ctrlType )
    {
        QSpinBox* pSpinBox = static_cast<QSpinBox*>(editor);
        pSpinBox->interpretText();
        int value = pSpinBox->value();
        model->setData(index, value, Qt::EditRole);
    }

    if( LineEdit == m_ctrlType )
    {
        QLineEdit* pLineEdit = static_cast<QLineEdit*>(editor);
        model->setData(index, pLineEdit->text(), Qt::EditRole);
    }
    qDebug()<<index.row()<<index.column()<<index.parent().row()<<index.parent().column();
}
//! [3]

//! [4]
void CtrlDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
//! [4]
