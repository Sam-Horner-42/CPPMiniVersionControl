#pragma once

#include <QtWidgets/QDialog>
#include "ui_SelectRepo.h"

class SelectRepo : public QDialog
{
    Q_OBJECT

public:
    SelectRepo(QWidget* parent = nullptr);
    ~SelectRepo();

private:
    Ui::SelectRepo ui;
};
