/* ============================================================================
 * Copyright (c) 2012 Michael A. Jackson (BlueQuartz Software)
 * Copyright (c) 2012 Dr. Michael A. Groeber (US Air Force Research Laboratories)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the name of Michael A. Groeber, Michael A. Jackson, the US Air Force,
 * BlueQuartz Software nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior written
 * permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  This code was written under United States Air Force Contract number
 *                           FA8650-07-D-5800
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "QIssuesDockWidget.h"

#include <iostream>

#include <QtGui/QLabel>
#include <QtGui/QTableWidgetItem>

#include "QtSupport/DREAM3DHelpUrlGenerator.h"

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QIssuesDockWidget::QIssuesDockWidget(QWidget* parent) :
  QDockWidget(parent)
{
  setupUi(this);
  setupGui();
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QIssuesDockWidget::~QIssuesDockWidget()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void QIssuesDockWidget::setupGui()
{
  errorTableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
  errorTableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Interactive);
  errorTableWidget->horizontalHeader()->resizeSection(1, 250);
  errorTableWidget->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
  errorTableWidget->verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  errorTableWidget->setShown(true);
  errorTableWidget->setVisible(true);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void QIssuesDockWidget::on_errorTableWidget_itemClicked( QTableWidgetItem* item )
{
  qDebug() << item->text().toLatin1().data() << "\n";
}


// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void QIssuesDockWidget::clearIssues()
{
  // std::cout << "QIssuesDockWidget::clearIssues()" << std::endl;
  for (int i = 0; i < errorTableWidget->rowCount(); ++i)
  {
    //  std::cout << "    Removing Issue" << std::endl;
    errorTableWidget->removeRow(i);
  }
  errorTableWidget->setRowCount(0);


}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void QIssuesDockWidget::processPipelineMessage(const PipelineMessage &msg)
{
  //  std::cout << "QIssuesDockWidget::processPipelineMessage(PipelineMessage& message)" << std::endl;
  // Create error hyperlink
  QLabel* hyperlinkLabel = createHyperlinkLabel(msg);

  QColor msgColor;
  switch(msg.getType())
  {
    case PipelineMessage::Error:
      // m_hasErrors = true;
      msgColor.setRed(255);
      msgColor.setGreen(191);
      msgColor.setBlue(193);
    {
      QBrush msgBrush(msgColor);

      QString msgDesc = (msg.getText());
      int msgCode = msg.getCode();

      int rc = errorTableWidget->rowCount();

      errorTableWidget->insertRow(rc);

      QString msgPrefix = (msg.getPrefix());
      QTableWidgetItem* filterNameWidgetItem = new QTableWidgetItem(msgPrefix);
      filterNameWidgetItem->setTextAlignment(Qt::AlignCenter);
      QTableWidgetItem* descriptionWidgetItem = new QTableWidgetItem(msgDesc);
      QTableWidgetItem* codeWidgetItem = new QTableWidgetItem(QString::number(msgCode));
      codeWidgetItem->setTextAlignment(Qt::AlignCenter);

      filterNameWidgetItem->setBackground(msgBrush);
      descriptionWidgetItem->setBackground(msgBrush);
      codeWidgetItem->setBackground(msgBrush);

      if (hyperlinkLabel == NULL)
      {
        errorTableWidget->setItem(rc, 0, filterNameWidgetItem);
      }
      else
      {
        errorTableWidget->setCellWidget(rc, 0, hyperlinkLabel);
      }
      errorTableWidget->setItem(rc, 1, descriptionWidgetItem);
      errorTableWidget->setItem(rc, 2, codeWidgetItem);
    }
      break;

    case PipelineMessage::Warning:
      //  m_hasWarnings = true;
      msgColor.setRed(251);
      msgColor.setGreen(254);
      msgColor.setBlue(137);

    {
      QBrush msgBrush(msgColor);

      QString msgName = (msg.getPrefix());
      QString msgDesc = (msg.getText());
      int msgCode = msg.getCode();

      int rc = errorTableWidget->rowCount();

      errorTableWidget->insertRow(rc);

      QTableWidgetItem* filterNameWidgetItem = new QTableWidgetItem(msgName);
      filterNameWidgetItem->setTextAlignment(Qt::AlignCenter);
      QTableWidgetItem* descriptionWidgetItem = new QTableWidgetItem(msgDesc);
      QTableWidgetItem* codeWidgetItem = new QTableWidgetItem(QString::number(msgCode));
      codeWidgetItem->setTextAlignment(Qt::AlignCenter);

      filterNameWidgetItem->setBackground(msgBrush);
      descriptionWidgetItem->setBackground(msgBrush);
      codeWidgetItem->setBackground(msgBrush);

      if (hyperlinkLabel == NULL)
      {
        errorTableWidget->setItem(rc, 0, filterNameWidgetItem);
      }
      else
      {
        errorTableWidget->setCellWidget(rc, 0, hyperlinkLabel);
      }
      errorTableWidget->setItem(rc, 1, descriptionWidgetItem);
      errorTableWidget->setItem(rc, 2, codeWidgetItem);
    }
      break;
#if 0
    case PipelineMessage::StatusValue:
      this->m_progressBar->setValue(msg.getProgressValue());
      break;
    case PipelineMessage::StatusMessage:
      if(NULL != this->statusBar())
      {
        QString s = (msg.getPrefix());
        s = s.append(" ").append(msg.getText().toLatin1().data());
        this->statusBar()->showMessage(s);
      }
      break;
    case PipelineMessage::StatusMessageAndValue:
      this->m_progressBar->setValue(msg.getProgressValue());
      if(NULL != this->statusBar())
      {
        QString s = (msg.getPrefix());
        s = s.append(" ").append(msg.getText().toLatin1().data());
        this->statusBar()->showMessage(s);
      }
      break;
#endif
    default:
      return;
  }


  //errorTableWidget->resizeRowsToContents();

}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
QLabel* QIssuesDockWidget::createHyperlinkLabel(PipelineMessage msg)
{
  QString filterClassName = (msg.getFilterClassName() );
  QString filterHumanLabel = (msg.getFilterHumanLabel() );
  QString msgPrefix = (msg.getPrefix());

  if ( filterClassName.isEmpty() || filterHumanLabel.isEmpty() )
  {
    if(filterClassName.isEmpty() == false) { return new QLabel(filterClassName); }
    else if(filterHumanLabel.isEmpty() == false) { return new QLabel(filterHumanLabel); }

    return new QLabel("Unknown Filter Class");
  }

  QUrl filterURL = DREAM3DHelpUrlGenerator::generateHTMLUrl( filterClassName.toLower() );
  QString filterHTMLText("<a href=\"");
  filterHTMLText.append(filterURL.toString()).append("\">").append(filterHumanLabel).append("</a>");

  QLabel* hyperlinkLabel = new QLabel(filterHTMLText);
  hyperlinkLabel->setTextFormat(Qt::RichText);
  hyperlinkLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
  hyperlinkLabel->setOpenExternalLinks(true);

  return hyperlinkLabel;
}