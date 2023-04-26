#include "huchart.h"
#include "ui_huchart.h"

HUChart::HUChart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HUChart)
{
    ui->setupUi(this);
}

HUChart::~HUChart()
{
    delete ui;
}
