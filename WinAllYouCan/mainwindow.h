#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAbstractTableModel>

namespace Ui {
class MainWindow;
}

class MyModel;
class RoundModel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int get_current_round() { return current_round; }
    int get_score(int round, int team);
    QString get_team_name(int team_number);

private slots:
    void on_button_start_game_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_show_score_clicked();
    void on_pushButton_team1_choice_clicked();
    void on_pushButton_team2_choice_clicked();
    void on_pushButton_team3_choice_clicked();
    void on_pushButton_team4_choice_clicked();
    void on_pushButton_team5_choice_clicked();
    void on_pushButton_team6_choice_clicked();
    void on_pushButton_team7_choice_clicked();
    void on_pushButton_team8_choice_clicked();
    void on_pushButton_next_clicked();
    void on_score_back_clicked();
    void on_score_next_clicked();
    void on_score_max_clicked();

private:
    Ui::MainWindow *ui;

    int setChoice(int team);
    void set_scores(int team, int ax_count);
    void clear_page();
    void show_score_totals();

    // button choices [round][team]
    int current_round;
    int choices[10][8];
    int scores[10][8];
    int round_multiplier;
    QIcon *ButtonIcons[3];
    MyModel *teama_model;
    MyModel *teamb_model;
    RoundModel *round_model;
    int teama_total;
    int teamb_total;
};

class RoundModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    RoundModel(QObject *parent);
    int rowCount(const QModelIndex &) const { return 12; }
    int columnCount(const QModelIndex &) const { return 1; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

class MyModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    MyModel(QObject *parent, MainWindow *mainWindow, int teamNumber);
    int rowCount(const QModelIndex &) const { return 12; }
    int columnCount(const QModelIndex &) const { return 4; }
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    MainWindow *main_window;
    int team_number;
};

#endif // MAINWINDOW_H
