#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QStringListModel>
#include <QVariant>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    current_round(0)
{
    // Clear the button choices
    memset(choices, -1, sizeof(choices));
    memset(scores, 0, sizeof(scores));

    // Get icons
    ButtonIcons[0] = new QIcon("../icons/Beads.png");
    ButtonIcons[1] = new QIcon("../icons/AxLog.png");
    ButtonIcons[2] = new QIcon("../icons/Question.png");

    QIcon WindowIcon("../icons/WinAllYouCan.ico");
    setWindowIcon(WindowIcon);

    // Setup UI
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    showMaximized();

    // Setup data models
    teama_model = new MyModel(0, this, 0);
    teamb_model = new MyModel(0, this, 1);
    round_model = new RoundModel(0);
    ui->tableView_teama->setModel(teama_model);
    ui->tableView_teamb->setModel(teamb_model);
    ui->tableView_rounds->setModel(round_model);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete ButtonIcons[0];
    delete ButtonIcons[1];
    delete ButtonIcons[2];

    delete teama_model;
    delete teamb_model;
}

int MainWindow::setChoice(int team)
{
    // rounds and teams are 1 based, so subtract 1 for the indexes
    int choice = choices[current_round][team - 1];
    choice = (choice + 1) % 2;
    choices[current_round][team - 1] = choice;
    return choice;
}

void MainWindow::on_button_start_game_clicked()
{
    // Setup page 1
    ui->team1_patrol->setText(ui->edit_patrol_team01->text());
    ui->team1_cool->setText(ui->edit_cool_team01->text());
    ui->team2_patrol->setText(ui->edit_patrol_team02->text());
    ui->team2_cool->setText(ui->edit_cool_team02->text());
    ui->team3_patrol->setText(ui->edit_patrol_team03->text());
    ui->team3_cool->setText(ui->edit_cool_team03->text());
    ui->team4_patrol->setText(ui->edit_patrol_team04->text());
    ui->team4_cool->setText(ui->edit_cool_team04->text());
    ui->team5_patrol->setText(ui->edit_patrol_team05->text());
    ui->team5_cool->setText(ui->edit_cool_team05->text());
    ui->team6_patrol->setText(ui->edit_patrol_team06->text());
    ui->team6_cool->setText(ui->edit_cool_team06->text());
    ui->team7_patrol->setText(ui->edit_patrol_team07->text());
    ui->team7_cool->setText(ui->edit_cool_team07->text());
    ui->team8_patrol->setText(ui->edit_patrol_team08->text());
    ui->team8_cool->setText(ui->edit_cool_team08->text());

    // Open the first round page
    ui->stackedWidget->setCurrentIndex(1);
}


void MainWindow::on_pushButton_back_clicked()
{
    // go back to setup
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButton_show_score_clicked()
{
    // Check for choices that havn't been made
    bool can_calculate_scores = true;
    for (int n = 0; n < 8; n++)
    {
        if (choices[current_round][n] < 0) {
            can_calculate_scores = false;
            break;
        }
    }

    if (can_calculate_scores) {
        // Calculate scores for this round
        // Start with team A
        int ax_count = 0;
        for (int n = 0; n < 4; n++)
            ax_count += choices[current_round][n];
        set_scores(0, ax_count);

        // Now get team b results
        ax_count = 0;
        for (int n = 0; n < 4; n++)
            ax_count += choices[current_round][4 + n];
        set_scores(1, ax_count);

        // Now display round scores
        ui->team1_score->setText(" " + QString::number(scores[current_round][0]));
        ui->team2_score->setText(" " + QString::number(scores[current_round][1]));
        ui->team3_score->setText(" " + QString::number(scores[current_round][2]));
        ui->team4_score->setText(" " + QString::number(scores[current_round][3]));
        ui->team5_score->setText(" " + QString::number(scores[current_round][4]));
        ui->team6_score->setText(" " + QString::number(scores[current_round][5]));
        ui->team7_score->setText(" " + QString::number(scores[current_round][6]));
        ui->team8_score->setText(" " + QString::number(scores[current_round][7]));
    }
    else {
        // set score fields to blanks
        ui->team1_score->setText("");
        ui->team2_score->setText("");
        ui->team3_score->setText("");
        ui->team4_score->setText("");
        ui->team5_score->setText("");
        ui->team6_score->setText("");
        ui->team7_score->setText("");
        ui->team8_score->setText("");
    }
}

void MainWindow::set_scores(int team, int ax_count)
{
    round_multiplier = 1;
    if (current_round >= 3 && current_round <= 5)
        round_multiplier = 2;
    else if (current_round >= 6 && current_round <= 8)
        round_multiplier = 5;
    else if (current_round == 9)
        round_multiplier = 10;

    switch (ax_count) {
        case 0: // all win 50 pts
            for (int n = 0; n < 4; n++) { scores[current_round][(team * 4) + n] = 50 * round_multiplier; }
            break;
        case 1: // ax +300 beads -100
            for (int n = 0; n < 4; n++)
            {
                if (choices[current_round][(team * 4) + n] > 0)
                    scores[current_round][(team * 4) + n] = 300 * round_multiplier;
                else
                    scores[current_round][(team * 4) + n] = -100 * round_multiplier;
            }
            break;
        case 2: // ax +200 beads -200
            for (int n = 0; n < 4; n++)
            {
                if (choices[current_round][(team * 4) + n] > 0)
                    scores[current_round][(team * 4) + n] = 200 * round_multiplier;
                else
                    scores[current_round][(team * 4) + n] = -200 * round_multiplier;
            }
            break;
        case 3: // ax +100 beads -300
            for (int n = 0; n < 4; n++)
            {
                if (choices[current_round][(team * 4) + n] > 0)
                    scores[current_round][(team * 4) + n] = 200 * round_multiplier;
                else
                    scores[current_round][(team * 4) + n] = -300 * round_multiplier;
            }
            break;
        case 4: // all lose -50
            for (int n = 0; n < 4; n++) { scores[current_round][(team * 4) + n] = -50 * round_multiplier; }
            break;
    }

    ui->pushButton_next->setEnabled(true);
}

void MainWindow::clear_page()
{
    // Set the round number
    QString round_number("");
    switch(current_round) {
        case 0:
            round_number = "One";
            ui->label_title->setText("The Game of Life");
            ui->label_tagline->setText("Objective: To Win All You Can!");
            break;
        case 1:
            round_number = "Two";
            ui->label_title->setText("The Game of Life");
            ui->label_tagline->setText("Objective: To Win All You Can!");
            break;
        case 2:
            round_number = "Three";
            ui->label_title->setText("The Game of Life");
            ui->label_tagline->setText("Objective: To Win All You Can!");
            break;
        case 3:
            round_number = "Four";
            ui->label_title->setText("The Game of Life 2x");
            ui->label_tagline->setText("Are you winning all you can?");
            break;
        case 4:
            round_number = "Five";
            ui->label_title->setText("The Game of Life 2x");
            ui->label_tagline->setText("WIN win win ALL all all YOU you you CAN can can");
            break;
        case 5:
            round_number = "Six";
            ui->label_title->setText("The Game of Life 2x");
            ui->label_tagline->setText("Win Win Win Win Win Win Win!");
            break;
        case 6:
            round_number = "Seven";
            ui->label_title->setText("The Game of Life 5x");
            ui->label_tagline->setText("Objective: To Win All You Can!");
            break;
        case 7:
            round_number = "Eight";
            ui->label_title->setText("The Game of Life 5x");
            ui->label_tagline->setText("Everybody, Win All You Can!");
            break;
        case 8:
            round_number = "Nine";
            ui->label_title->setText("The Game of Life 5x");
            ui->label_tagline->setText("Win ALL You Can!");
            break;
        case 9:
            round_number = "Ten";
            ui->label_title->setText("The Game of Life 10x");
            ui->label_tagline->setText("Winners, Win All You Can!");
            break;
    }
    ui->label_round_number->setText(round_number);

    // Clear the page and setup for round
    ui->pushButton_team1_choice->setIcon(*ButtonIcons[choices[current_round][0] < 0 ? 2 : choices[current_round][0]]);
    ui->pushButton_team2_choice->setIcon(*ButtonIcons[choices[current_round][1] < 0 ? 2 : choices[current_round][1]]);
    ui->pushButton_team3_choice->setIcon(*ButtonIcons[choices[current_round][2] < 0 ? 2 : choices[current_round][2]]);
    ui->pushButton_team4_choice->setIcon(*ButtonIcons[choices[current_round][3] < 0 ? 2 : choices[current_round][3]]);
    ui->pushButton_team5_choice->setIcon(*ButtonIcons[choices[current_round][4] < 0 ? 2 : choices[current_round][4]]);
    ui->pushButton_team6_choice->setIcon(*ButtonIcons[choices[current_round][5] < 0 ? 2 : choices[current_round][5]]);
    ui->pushButton_team7_choice->setIcon(*ButtonIcons[choices[current_round][6] < 0 ? 2 : choices[current_round][6]]);
    ui->pushButton_team8_choice->setIcon(*ButtonIcons[choices[current_round][7] < 0 ? 2 : choices[current_round][7]]);

    on_pushButton_show_score_clicked();
    ui->pushButton_next->setEnabled(false);

    ui->pushButton_team1_choice->setFocus();
}

void MainWindow::on_pushButton_team1_choice_clicked()
{
    int choice = setChoice(1);
    ui->pushButton_team1_choice->setIcon(*ButtonIcons[choice]);
}


void MainWindow::on_pushButton_team2_choice_clicked()
{
    int choice = setChoice(2);
    ui->pushButton_team2_choice->setIcon(*ButtonIcons[choice]);
}

void MainWindow::on_pushButton_team3_choice_clicked()
{
    int choice = setChoice(3);
    ui->pushButton_team3_choice->setIcon(*ButtonIcons[choice]);
}

void MainWindow::on_pushButton_team4_choice_clicked()
{
    int choice = setChoice(4);
    ui->pushButton_team4_choice->setIcon(*ButtonIcons[choice]);
}

void MainWindow::on_pushButton_team5_choice_clicked()
{
    int choice = setChoice(5);
    ui->pushButton_team5_choice->setIcon(*ButtonIcons[choice]);
}

void MainWindow::on_pushButton_team6_choice_clicked()
{
    int choice = setChoice(6);
    ui->pushButton_team6_choice->setIcon(*ButtonIcons[choice]);
}

void MainWindow::on_pushButton_team7_choice_clicked()
{
    int choice = setChoice(7);
    ui->pushButton_team7_choice->setIcon(*ButtonIcons[choice]);
}

void MainWindow::on_pushButton_team8_choice_clicked()
{
    int choice = setChoice(8);
    ui->pushButton_team8_choice->setIcon(*ButtonIcons[choice]);
}

void MainWindow::on_pushButton_next_clicked()
{
    // Move to the next round or to the score summary page
    switch (current_round) {
    case 0:
    case 1:
        // Just move to the next page
        current_round++;
        clear_page();
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        // Show score sheet
        show_score_totals();
        break;
    }
}

void MainWindow::on_score_back_clicked()
{
    // Go back to score sheet
    current_round--;
    clear_page();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_score_next_clicked()
{
    // Move on to next round
    current_round++;
    clear_page();
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_score_max_clicked()
{
    if (ui->score_max->text().contains("Team Total")) {
        // Show the team totals, and change the button name to max
        ui->score_max->setText("&Combined");
        ui->label_teama_title->setText("Team A");
        ui->label_teamb_title->setText("Team B");
        teama_total = 0;
        teamb_total = 0;
        for (int n = 0; n < 10; n++) {
            teama_total += scores[n][0];
            teama_total += scores[n][1];
            teama_total += scores[n][2];
            teama_total += scores[n][3];
            teamb_total += scores[n][4];
            teamb_total += scores[n][5];
            teamb_total += scores[n][6];
            teamb_total += scores[n][7];
        }
        ui->label_MaxScoreA->setText(QString::number(teama_total));
        ui->label_MaxScoreB->setText(QString::number(teamb_total));
        ui->label_CombinedTotal->setText("&Team Total");
    }
    else if (ui->score_max->text().contains("Combined")) {
        // Show the max possible scores
        ui->score_max->setText("&Max");
        ui->label_CombinedTotal->setText("-> " + QString::number(teama_total + teamb_total) + " <-");
    }
    else if (ui->score_max->text().contains("Max")) {
        // Show the max possible scores
        ui->score_max->setEnabled(false);
        ui->label_MaxScoreA->setText(QString::number(6800));
        ui->label_MaxScoreB->setText(QString::number(6800));
        ui->label_CombinedTotal->setText("-> 13,600 <-");
    }
}

void MainWindow::show_score_totals()
{
    ui->stackedWidget->setCurrentIndex(2);

    QSize size_a = ui->tableView_teama->size();
    QSize size_b = ui->tableView_teamb->size();
    QSize size_rounds = ui->tableView_rounds->size();
    for (int n = 0; n < 4; n++) {
        ui->tableView_teama->setColumnWidth(n, size_a.width() / 4);
        ui->tableView_teamb->setColumnWidth(n, size_b.width() / 4);
    }
    for (int n = 0; n < 12; n++) {
        ui->tableView_teama->setRowHeight(n, size_a.height() / 12);
        ui->tableView_teamb->setRowHeight(n, size_b.height() / 12);
        ui->tableView_rounds->setRowHeight(n, size_rounds.height() / 12);
    }
    ui->tableView_rounds->setColumnWidth(0, size_rounds.width());

    if (current_round == 9) {
        ui->score_next->setEnabled(false);
        ui->score_next->setVisible(false);
        ui->score_max->setVisible(true);
        ui->score_max->setEnabled(true);
        ui->score_max->setText("Team Totals");
    }
    else {
        ui->score_next->setEnabled(true);
        ui->score_next->setVisible(true);
        ui->score_max->setVisible(false);
    }
}

int MainWindow::get_score(int round, int team)
{
    return scores[round][team];
}

QString MainWindow::get_team_name(int team_number)
{
    switch(team_number) {
        case 0: return ui->edit_cool_team01->text(); break;
        case 1: return ui->edit_cool_team02->text(); break;
        case 2: return ui->edit_cool_team03->text(); break;
        case 3: return ui->edit_cool_team04->text(); break;
        case 4: return ui->edit_cool_team05->text(); break;
        case 5: return ui->edit_cool_team06->text(); break;
        case 6: return ui->edit_cool_team07->text(); break;
        case 7: return ui->edit_cool_team08->text(); break;
    }

    return QString();
}

RoundModel::RoundModel(QObject *parent):
    QAbstractTableModel(parent)
{
}

QVariant RoundModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    else if (role != Qt::DisplayRole)
        return QVariant();

    if (index.row() == 0)
        return QString("");
    else if (index.row() < 11)
        return QString("Round " + QString::number(index.row()));
    else
        return QString("Total");

    return QString("");
}


MyModel::MyModel(QObject *parent, MainWindow *mainWindow, int teamNumber):
    QAbstractTableModel(parent),
    main_window(mainWindow),
    team_number(teamNumber)
{
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if (main_window == NULL)
        return QVariant();

    int round_number = index.row() - 1;
    int team = (team_number * 4) + index.column();

    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    else if (role == Qt::ForegroundRole)
    {
        // Determine if this is a score cell - if it is and the score is negative
        // then set the foreground color to red, else black
        if (round_number >= 0 && round_number < 10 && round_number <= main_window->get_current_round()) {
            if (main_window->get_score(round_number, team) < 0)
                return QColor(Qt::red);
            else
                return QColor(Qt::black);
        }
        else if(round_number == 10) {
            // calculate total
            int total = 0;
            for (int round = 0; round < 10; round++) {
                total += main_window->get_score(round, team);
            }
            if (total < 0)
                return QColor(Qt::red);
            else
                return QColor(Qt::black);
        }
    }
    else if(role != Qt::DisplayRole)
        return QVariant();

    if (index.row() == 0) {
        return main_window->get_team_name(team);
    }
    else if (round_number < 10) {
        switch(index.column()) {
            case 0:
            case 1:
            case 2:
            case 3:
                if (round_number <= main_window->get_current_round()) {
                    return main_window->get_score(round_number, team);
                }
                else {
                    return QString("");
                }
                break;
            default:
                return QVariant();
                break;
        }
    }
    else {
        // Totals row
        switch(index.column()) {
            case 0:
            case 1:
            case 2:
            case 3:
                {
                    int total = 0;
                    for (int round = 0; round < 10; round++) {
                        total += main_window->get_score(round, team);
                    }
                    return QString::number(total);
                }
                break;
            default:
                return QVariant();
                break;
        }
    }

    return QVariant();
}

