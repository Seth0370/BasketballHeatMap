#include <iostream>
#include <stack>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

struct stats
{
    string name;
    int m = 0;
    int att = 0;
};

struct ShotEntry
{
    string zoneName;
    int m;
    int att;
};

struct ShootingMap
{
    vector<vector<stats>> court =
    {
        { {"Left Corner"}, {"Left Wing"}, {"Top Key"}, {"Right Wing"}, {"Right Corner"} },
        { {"Left Elbow"}, {"Left Free Throw"}, {"Top Paint"}, {"Right Free Throw"}, {"Right Elbow"} },
        { {"Left Paint"}, {"Low Paint Left"}, {"Restricted Area"}, {"Low Paint Right"}, {"Right Paint"} }
    };

    stack<ShotEntry> hist;

    stats* find(const string& loc)
    {
        for (size_t i = 0; i < court.size(); i++)
        {
            for (size_t j = 0; j < court[i].size(); j++)
            {
                if (court[i][j].name == loc)
                {
                    return &court[i][j];
                }
            }
        }
        return nullptr;
    }

    void record(const string& loc, int m, int att)
    {
        stats* zone = find(loc);
        if (zone)
        {
            zone->m += m;
            zone->att += att;
            hist.push({ loc, m, att });
        }
    }

    string undo()
    {
        if (!hist.empty())
        {
            ShotEntry last_entry = hist.top();
            hist.pop();

            stats* zone = find(last_entry.zoneName);
            if (zone)
            {
                zone->m -= last_entry.m;
                zone->att -= last_entry.att;
            }
            return last_entry.zoneName;
        }
        else
        {
            cout << "No entries to undo!\n";
            return "";
        }
    }

    void display()
    {
        cout << "\n------------------------------------------------------------------\n";
        for (size_t i = 0; i < court.size(); i++)
        {
            for (size_t j = 0; j < court[i].size(); j++)
            {
                stats& zone = court[i][j];
                double perc;
                if (zone.att > 0)
                {
                    perc = (zone.m * 100.0 / zone.att);
                }
                else
                {
                    perc = 0;
                }
                cout << "| " << setw(18) << left << zone.name << " "
                    << setw(3) << fixed << setprecision(0) << perc << "% "
                    << setw(2) << zone.m << "/" << setw(2) << zone.att << " ";
                if (j == 2 || j == 4)
                {
                    cout << "|\n";
                }
            }
            cout << "------------------------------------------------------------------\n";
        }
    }

    void total()
    {
        int total_m = 0, total_att = 0;
        for (size_t i = 0; i < court.size(); i++)
        {
            for (size_t j = 0; j < court[i].size(); j++)
            {
                total_m += court[i][j].m;
                total_att += court[i][j].att;
            }
        }
        double total_perc;
        if (total_att > 0)
        {
            total_perc = (total_m * 100.0 / total_att);
        }
        else
        {
            total_perc = 0;
        }
        cout << "\nTotal Shooting Percentage: " << fixed << setprecision(2) << total_perc << "% (" << total_m << "/" << total_att << ")\n";
    }

    void tot_points()
    {
        int total_points = 0;
        for (size_t i = 0; i < court.size(); i++)
        {
            for (size_t j = 0; j < court[i].size(); j++)
            {
                if (i == 0)
                {
                    total_points += court[i][j].m * 3;
                }
                else
                {
                    total_points += court[i][j].m * 2;
                }
            }
        }
        cout << "Total Points Scored: " << total_points << "\n";
    }

};

int main()
{
    ShootingMap sm;
    string input;
    int m, att;
    char separator;
    string re_enter = "";
    cout << "Enter makes and misses in a 'X/Y' format (Type 'U' to undo last entry)\n";
    for (size_t i = 0; i < sm.court.size(); i++)
    {
        for (size_t j = 0; j < sm.court[i].size(); j++)
        {
            stats& zone = sm.court[i][j];
            bool valid = false;
            while (!valid)
            {
                string current = re_enter.empty() ? zone.name : re_enter;
                cout << current << ": ";
                getline(cin, input);
                if (input == "U" || input == "u")
                {
                    re_enter = sm.undo();
                    if (!re_enter.empty())
                    {
                        cout << "Undo successful! Re-enter value for " << re_enter << ".\n";
                    }
                    j--;
                    continue;
                }
                stringstream ss(input);
                if (ss >> m >> separator >> att && separator == '/')
                {
                    if (att == 0 && m > 0)
                    {
                        cout << "Invalid input! If attempts are 0, makes must also be 0.\n";
                    }
                    else if (m > att || att < 0)
                    {
                        cout << "Invalid input! Makes cannot be greater than attempts.\n";
                    }
                    else
                    {
                        sm.record(current, m, att);
                        valid = true;
                        re_enter = "";
                    }
                }
                else
                {
                    cout << "Invalid format! Use 'X/Y'. Try again.\n";
                }
            }
        }
    }
    sm.display();
    sm.total();
    sm.tot_points();
    return 0;
}