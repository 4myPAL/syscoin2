#ifndef OFFERACCEPTINFODIALOG_H
#define OFFERACCEPTINFODIALOG_H
#include <QDialog>
class QDataWidgetMapper;
namespace Ui {
    class OfferAcceptInfoDialog;
}
QT_BEGIN_NAMESPACE
class QModelIndex;
QT_END_NAMESPACE
/** Dialog for editing an address and associated information.
 */
class OfferAcceptInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OfferAcceptInfoDialog(const QModelIndex &idx, QWidget *parent=0);
    ~OfferAcceptInfoDialog();
private:
	bool lookup();
private Q_SLOTS:
	void on_okButton_clicked();
private:
	QDataWidgetMapper *mapper;
    Ui::OfferAcceptInfoDialog *ui;
	QString offerGUID;
	QString offerAcceptGUID;

};

#endif // OFFERACCEPTINFODIALOG_H
