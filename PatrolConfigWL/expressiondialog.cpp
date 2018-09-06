#include "expressiondialog.h"
#include "ui_expressiondialog.h"
#include <QMessageBox>

ExpressionDialog::ExpressionDialog(QString express, QString expressItem, QSqlDatabase *pdb, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExpressionDialog)
{
    ui->setupUi(this);

    m_express = express;
    m_expressItem = expressItem;
	m_pdb = pdb;

	decoding();


	channelModel = new QSqlTableModel(this,*m_pdb);
    channelModel->setTable("channelinfo");
    channelModel->setHeaderData(2, Qt::Horizontal, trUtf8("端口编号"));
    channelModel->setHeaderData(3, Qt::Horizontal, trUtf8("端口类型"));
    channelModel->setHeaderData(4, Qt::Horizontal, trUtf8("端口名称"));
    channelModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    ui->tableChannel->setModel(channelModel);
    ui->tableChannel->setEditTriggers(QTableView::NoEditTriggers);
    for (int i = 0; i < 28; i++)
    {
        if (i == 2 || i == 3 || i == 4)
            continue;
        ui->tableChannel->setColumnHidden(i, true);
    }

	devModel = new QSqlTableModel(this,*m_pdb);
    devModel->setTable("terminalinfo");
    devModel->select();
    ui->cboDev->setModel(devModel);
    ui->cboDev->setModelColumn(1);

}

ExpressionDialog::~ExpressionDialog()
{
    delete ui;
}

void ExpressionDialog::on_cboDev_currentIndexChanged(int index)
{
    int nTerId = devModel->record(index).value(0).toInt();
    QString sType = ui->cboType->currentText();
    QString filter = QString("C_TerID=%1 and C_ChannelType='%2'").arg(nTerId).arg(sType);
    channelModel->setFilter(filter);
    channelModel->select();
}

void ExpressionDialog::on_cboType_currentIndexChanged(const QString &arg1)
{
    int nTerId = devModel->record(ui->cboDev->currentIndex()).value(0).toInt();
    QString filter = QString("C_TerID=%1 and C_ChannelType='%2'").arg(nTerId).arg(arg1);
    channelModel->setFilter(filter);
    channelModel->select();
}

void ExpressionDialog::on_btnD_clicked()
{
    addExpression(trUtf8("当"));
}

void ExpressionDialog::on_textEdit_textChanged()
{
    QString text = ui->textEdit->toPlainText();
    if (text.indexOf(trUtf8("当")) != -1)
    {
        ui->btnD->setEnabled(false);
        ui->btnHe->setEnabled(true);
        ui->btnHuo->setEnabled(true);
    }
    else
    {
        ui->btnD->setEnabled(true);
        ui->btnHe->setEnabled(false);
        ui->btnHuo->setEnabled(false);
    }
}

void ExpressionDialog::on_btnHe_clicked()
{
    addExpression(trUtf8("和"));

}

void ExpressionDialog::on_btnHuo_clicked()
{
    addExpression(trUtf8("或"));
}

void ExpressionDialog::on_btnQF_clicked()
{
    QString tmp_src  = ui->textEdit->textCursor().selectedText();
    QString tmp_src_copy;

    int index_begin = 0;
    int index_end = 0;

    QString tmpstr;
    QString tmpstr2;

    while((tmp_src.indexOf("[") != -1) && (tmp_src.indexOf("]") != -1))
    {
        index_begin = tmp_src.indexOf("[");
        index_end = tmp_src.indexOf("]");

        tmpstr = tmp_src.mid(index_begin, index_end - index_begin + 1);

        //处理
//        tmpstr2 = substitute(tmpstr);
        QString selectedText = tmpstr;

        QString val = m_expression[selectedText];
        m_expression[selectedText] = "";

        if (selectedText.contains("告警"))
        {
            selectedText.replace("=告警", "=正常");
            val = "(!" + val + ")";
        }
        else if(selectedText.contains("正常"))
        {
            selectedText.replace("=正常", "=告警");
            val.replace("(!", "");
            val.replace(")", "");
        }
        m_expression[selectedText] = val;

        //

        tmp_src_copy += tmp_src.left(index_begin) + selectedText;
        tmp_src.remove(0, index_end+1);
    }

    tmp_src_copy += tmp_src;

    //替换显示文本
    QString text = ui->textEdit->toPlainText();

    ui->textEdit->textCursor().insertText(tmp_src_copy);
//    QString selectedText = ui->textEdit->textCursor().selectedText();
//    selectedText = selectedText.replace("或","和");
//    selectedText = selectedText.simplified();

//    QStringList strlist = selectedText.split("和");

//    for(int a=0;a<strlist.count();a++)
//    {
//        strlist[a] = strlist[a].trimmed();
//        QString tmptext = strlist[a];
//        tmptext = tmptext.replace("","");
//        if(tmptext.length()>2)
//        {
//            QString val = m_expression[tmptext];
//            m_expression[tmptext] = "";

//            if (tmptext.contains("告警"))
//            {
//                tmptext.replace("=告警", "=正常");
//                val = "(!" + val + ")";
//            }
//            else if(tmptext.contains("正常"))
//            {
//                tmptext.replace("=正常", "=告警");
//                val.replace("(!", "");
//                val.replace(")", "");
//            }

//            m_expression[tmptext] = val;
//            selectedText.replace(strlist[a],tmptext);
//        }
//    }

//    QString text = ui->textEdit->toPlainText();
//    text.replace(ui->textEdit->textCursor().selectedText(), selectedText);
//    ui->textEdit->setText(text);
}

void ExpressionDialog::addExpression(QString s)
{
    if (ui->tableChannel->currentIndex().row() == -1)
        return;

    int n = ui->tableChannel->selectedIndexes().count();
    for(int i=0;i<n;i++)
    {
        int nRow = ui->tableChannel->currentIndex().row();
        nRow = ui->tableChannel->selectedIndexes().at(i).row();
        QString dev = ui->cboDev->currentText();
        QString channel = channelModel->record(nRow).value(4).toString();
        QString keyword = trUtf8("[%1.%2的状态=告警]").arg(dev).arg(channel);
        channel = QString("#%1#").arg(channelModel->record(nRow).value(0).toString());
        if (!m_expression.regKeyword(keyword, channel))
        {
            QMessageBox::information(this, trUtf8("友情提示"), trUtf8("该端口已经被添加!"));
            return;
        }
        if(s == QString("当") && i>0)
        {
            s = QString("和");
        }
        ui->textEdit->append(s);
        ui->textEdit->append(keyword);
    }
}

void ExpressionDialog::decoding()
{
    if (m_express.isEmpty() || m_expressItem.isEmpty())
        return;
    QString sQuery = "select DBIndex,T_Name, C_ChannelName from channelinfo c "
            "inner join terminalinfo t on c.C_TerID=t.T_ID where DBIndex in (%1)";
    QStringList expressItems = m_expressItem.split(";");
    if (expressItems.last().isEmpty())
        expressItems.removeLast();
    for(int a=0;a<expressItems.count();a++)
    {
        if(expressItems.at(a).length()==0)
            expressItems.removeAt(a);
    }
    QString sParam = expressItems.join(",");
    sQuery = sQuery.arg(sParam);
    QString exp = m_express;
	QSqlQuery query(*m_pdb);
    query.exec(sQuery);
    while (query.next())
    {
        QString key = "#" + query.record().value(0).toString() + "#";
        QString key1 = "(!#" + query.record().value(0).toString() + "#)";
        QString name = trUtf8("[%1.%2的状态=告警]").
                arg(query.record().value(1).toString()).
                arg(query.record().value(2).toString());
        if (exp.indexOf(key1) != -1)
        {
            name = trUtf8("[%1.%2的状态=正常]").
                    arg(query.record().value(1).toString()).
                    arg(query.record().value(2).toString());
            exp.replace(key1, name);
            m_expression.regKeyword(name, key1);
        }
        else
        {
            exp.replace(key, name);
            m_expression.regKeyword(name, key);
        }
    }
    m_expression.decoding(exp);
    ui->textEdit->setText(exp);
}

void ExpressionDialog::on_btnClear_clicked()
{
    ui->textEdit->clear();
    m_expression.clear();
}

void ExpressionDialog::on_btnKH_clicked()
{
    QString selectedText = ui->textEdit->textCursor().selectedText();
    if (selectedText.left(1) == "(" && selectedText.right(1) == ")")
    {
        QString text = ui->textEdit->toPlainText();
        text.remove(ui->textEdit->textCursor().selectionEnd()-1, 1);
        text.remove(ui->textEdit->textCursor().selectionStart(), 1);
        ui->textEdit->setText(text);
        return;
    }
    if (selectedText.left(1) != "[" || selectedText.right(1) != "]")
    {
        QMessageBox::information(this, trUtf8("友情提示"), trUtf8("选取格式[*]"));
        return;
    }
    QString text = ui->textEdit->toPlainText();
    text.insert(ui->textEdit->textCursor().selectionStart(), "(");
    text.insert(ui->textEdit->textCursor().selectionEnd()+1, ")");
    ui->textEdit->setText(text);
}

void ExpressionDialog::on_btnOk_clicked()
{
    m_express = ui->textEdit->toPlainText();
    m_expression.getExpression(m_express, m_expressItem);
    accept();
}

void ExpressionDialog::on_btnDel_clicked()
{
    QString selectedText = ui->textEdit->textCursor().selectedText();
    QRegExp exp("\\[[^\\]]*\\]");
    int nPos = 0;
    while ((nPos = exp.indexIn(selectedText, nPos)) != -1)
    {
        QStringList texts = exp.capturedTexts();
        foreach (QString keyword, texts)
            m_expression.removeKeyword(keyword);
        nPos += exp.matchedLength();
    }
    QString text = ui->textEdit->toPlainText();
    text.remove(ui->textEdit->textCursor().selectionStart()-1, selectedText.length() + 1);
    ui->textEdit->setText(text);
}

void ExpressionDialog::on_textEdit_selectionChanged()
{
    QString selectedText = ui->textEdit->textCursor().selectedText().trimmed();
    qDebug() << selectedText;
    selectedText.replace('#','￥');
    if ((selectedText.left(1) == "(" && selectedText.right(1) == ")") ||
            (selectedText.left(1) == "[" && selectedText.right(1) == "]"))
    {
        ui->btnKH->setEnabled(true);
    }
    else
    {
        ui->btnKH->setEnabled(false);
    }
    if(selectedText.contains("告警"))
    {
        selectedText = selectedText.replace("告警","1");
    }
    else if(selectedText.contains("正常"))
    {
        selectedText = selectedText.replace("正常","0");
    }
    qDebug()<<selectedText;
    QRegExp exp("^\\[[\\u4e00-\\u9fa5\\w-]*\\.[\\u4e00-\\u9fa5\\w-]*=[01]\\]$");
    if (exp.exactMatch(selectedText))
    {
        ui->btnQF->setEnabled(true);

    }
    else
    {
        ui->btnQF->setEnabled(false);
    }
    QString pattern = exp.pattern();
    pattern = trUtf8("^[和或](\\n|.)*\\]$");
    exp.setPattern(pattern);
    if (exp.exactMatch(selectedText))
    {
        ui->btnDel->setEnabled(true);
    }
    else
    {
        ui->btnDel->setEnabled(false);
    }

    ui->btnQF->setEnabled(true);
}

void ExpressionDialog::on_pushButton_clicked()
{
    this->close();
}
