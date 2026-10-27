#include <iostream>
using namespace std;
const int MAX = 20;
const int REAL_PLACES = 12;
int N = 12;
const float INF = 99999;
char locationName[20][45] = {
    "Amritsar Group of Colleges (AGC)",
    "Golden Temple",
    "Amritsar Railway Station",
    "Bus Stand",
    "Town Hall Fire Station",
    "Fire Station Amritsar (Mahan Singh Gate)",
    "Fire Station Jandiala Guru",
    "Guru Nanak Dev Hospital",
    "Fortis Escorts Hospital",
    "Surinder Hospital",
    "Jandiala Guru Police Station",
    "Rambagh Police Station"};
int locationType[20] = {0, 0, 0, 0, 2, 2, 2, 1, 1, 1, 3, 3};
float road[20][20];
int status[20][20];
void makeMap()
{
    for (int i = 0; i < MAX; i++)
    {
        for (int j = 0; j < MAX; j++)
        {
            if (i == j)
            {
                road[i][j] = 0;
            }
            else
            {
                road[i][j] = INF;
            }
            status[i][j] = 0;
        }
    }
}
void addRoad(int a, int b, float km)
{
    road[a][b] = km;
    road[b][a] = km;
}
void createRoads()
{
    addRoad(0, 9, 22);
    addRoad(0, 2, 18);
    addRoad(0, 3, 12);
    addRoad(0, 5, 13);
    addRoad(0, 4, 15);
    addRoad(0, 8, 16);
    addRoad(0, 7, 16);
    addRoad(0, 10, 4.8);
    addRoad(0, 6, 5.9);
    addRoad(3, 2, 2.3);
    addRoad(2, 4, 2.5);
    addRoad(4, 1, 1.7);
    addRoad(1, 2, 3.3);
    addRoad(1, 7, 4.9);
    addRoad(2, 7, 4.5);
    addRoad(7, 8, 5.4);
    addRoad(8, 2, 7.9);
    addRoad(9, 2, 8.5);
    addRoad(5, 3, 0.8);
    addRoad(10, 1, 17);
    addRoad(10, 2, 18);
    addRoad(11, 1, 2.1);
    addRoad(11, 2, 1.2);
}
void printType(int t)
{
    if (t == 1)
        cout << "Hospital";
    else if (t == 2)
        cout << "Fire Station";
    else if (t == 3)
        cout << "Police Station";
    else
        cout << "Normal";
}
void printStatus(int s)
{
    if (s == 1)
        cout << "Traffic (takes double time)";
    else if (s == 2)
        cout << "Accident ";
    else if (s == 3)
        cout << "Construction (road may be closed)";
    else
        cout << "Clear";
}
float estimateTravelMinutes(float km)
{
   
    return km * 2.0f;
}
void printTime(float minutes)
{
    cout << "rough travel-time estimate: about " << (int)(minutes + 0.5f)
         << " min (2 min/km; condition delays not included)";
}
void showMap()
{
    cout << "\n CITY LOCATIONS (AMRITSAR) \n";
    for (int i = 0; i < N; i++)
    {
        cout << (i + 1) << ". " << locationName[i] << " | Type: ";
        printType(locationType[i]);
        cout << endl;
    }
    cout << "\n CITY ROADS \n";
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            if (road[i][j] != INF)
            {
                count++;
                cout << count << ". " << locationName[i] << " <-> "
                     << locationName[j] << " | " << road[i][j]
                     << " km | Status: ";
                printStatus(status[i][j]);
                cout << endl;
            }
        }
    }
    cout << "\nTotal Locations: " << N << endl;
    cout << "Total Roads: " << count << endl;
}
float dist[20];
int parent[20];
int done[20];
int findClosest()
{
    int best = -1;
    for (int i = 0; i < N; i++)
    {
        if (done[i] == 0)
        {
            if (best == -1 || dist[i] < dist[best])
            {
                best = i;
            }
        }
    }
    return best;
}
float roadCost(int a, int b)
{
    if (road[a][b] == INF || status[a][b] == 3)
    {
        return INF;
    }
    if (status[a][b] == 1)
    {
        return road[a][b] * 2;
    }
    if (status[a][b] == 2)
    {
        return road[a][b] * 3;
    }
    return road[a][b];
}
void dijkstra(int start)
{
    for (int i = 0; i < N; i++)
    {
        dist[i] = INF;
        parent[i] = -1;
        done[i] = 0;
    }
    dist[start] = 0;
    for (int round = 0; round < N; round++)
    {
        int u = findClosest();
        if (u == -1 || dist[u] == INF)
        {
            break;
        }
        done[u] = 1;
        for (int v = 0; v < N; v++)
        {
            float cost = roadCost(u, v);
            if (cost != INF && dist[u] + cost < dist[v])
            {
                dist[v] = dist[u] + cost;
                parent[v] = u;
            }
        }
    }
}
void showRoute(int end)
{
    int path[20];
    int count = 0;
    int current = end;
    while (current != -1)
    {
        path[count] = current;
        count++;
        current = parent[current];
    }
    cout << locationName[path[count - 1]] << endl;
    for (int i = count - 2; i >= 0; i--)
    {
        cout << "      -- " << road[path[i + 1]][path[i]] << " km --> "
             << locationName[path[i]] << endl;
    }
}
float routeDistance(int end)
{
    
    float km = 0;
    int current = end;
    while (parent[current] != -1)
    {
        int previous = parent[current];
        km += road[previous][current];
        current = previous;
    }
    return km;
}
int histPlace[10];
int histHelp[10];
int histFound[10];
float histDist[10];
float histTime[10];
int histCount = 0;
void saveHistory(int place, int wanted, int found, float km, float minutes)
{
    int slot = histCount % 10;
    histPlace[slot] = place;
    histHelp[slot] = wanted;
    histFound[slot] = found;
    histDist[slot] = km;
    histTime[slot] = minutes;
    histCount++;
}
void showHistory()
{
    cout << "\nRECENT EMERGENCIES \n";
    if (histCount == 0)
    {
        cout << "No emergencies reported yet.\n";
        return;
    }
    int shown = histCount;
    if (shown > 10)
    {
        shown = 10;
    }
    for (int k = 0; k < shown; k++)
    {
        int slot = (histCount - 1 - k) % 10;
        cout << (k + 1) << ". Emergency at " << locationName[histPlace[slot]]
             << " | Help needed: ";
        printType(histHelp[slot]);
        cout << "\n   Sent to: ";
        if (histFound[slot] == -1)
        {
            cout << "no facility could be reached\n";
        }
        else
        {
            cout << locationName[histFound[slot]] << " (" << histDist[slot] << " actual km; ";
            printTime(histTime[slot]);
            cout << ")\n";
        }
    }
}
void findFacility(int emergency, int wanted)
{
    dijkstra(emergency);
    int facList[20];
    int facCount = 0;
    for (int i = 0; i < N; i++)
    {
        if (locationType[i] == wanted && dist[i] != INF)
        {
            facList[facCount] = i;
            facCount++;
        }
    }
    if (facCount == 0)
    {
        cout << "\nNo facility of this type can be reached!\n";
        saveHistory(emergency, wanted, -1, 0, 0);
        return;
    }
    for (int i = 0; i < facCount - 1; i++)
    {
        for (int j = 0; j < facCount - 1 - i; j++)
        {
            if (dist[facList[j]] > dist[facList[j + 1]])
            {
                int temp = facList[j];
                facList[j] = facList[j + 1];
                facList[j + 1] = temp;
            }
        }
    }
    cout << "\n HELP FOR EMERGENCY AT: " << locationName[emergency] << " \n";
    for (int i = 0; i < facCount; i++)
    {
        if (i == 0)
        {
            cout << "\nNEAREST  : ";
        }
        else
        {
            cout << "\nBACKUP " << i << " : ";
        }
        float actualKm = routeDistance(facList[i]);
        float roughMinutes = estimateTravelMinutes(actualKm);
        cout << locationName[facList[i]] << " (" << actualKm << " actual km; ";
        printTime(roughMinutes);
        cout << ")\n";
        cout << "Route    : ";
        showRoute(facList[i]);
    }
    if (facCount > 1)
    {
        cout << "\nIf the nearest one cannot help, go to the backup options above.\n";
    }
    float nearestKm = routeDistance(facList[0]);
    saveHistory(emergency, wanted, facList[0], nearestKm,
                estimateTravelMinutes(nearestKm));
}
void reportEmergency()
{
    int place, wanted;
    cout << "\nWhere is the emergency?\n";
    for (int i = 0; i < N; i++)
    {
        cout << "  " << (i + 1) << " = " << locationName[i] << endl;
    }
    cout << "Enter place number (1 = AGC college): ";
    cin >> place;
    place = place - 1;
    cout << "What help is needed? (1 = Hospital, 2 = Fire Station, 3 = Police): ";
    cin >> wanted;
    if (place < 0 || place >= N || wanted < 1 || wanted > 3)
    {
        cout << "Wrong input!\n";
        return;
    }
    findFacility(place, wanted);
}
const int UNDO_SIZE = 100;
int undoA[UNDO_SIZE];
int undoB[UNDO_SIZE];
int undoOld[UNDO_SIZE];
int top = 0;
void pushUndo(int a, int b, int oldStatus)
{
    if (top < UNDO_SIZE)
    {
        undoA[top] = a;
        undoB[top] = b;
        undoOld[top] = oldStatus;
        top++;
    }
}
void undoLast()
{
    if (top == 0)
    {
        cout << "\nNothing to undo.\n";
        return;
    }
    top--;
    int a = undoA[top];
    int b = undoB[top];
    int old = undoOld[top];
    status[a][b] = old;
    status[b][a] = old;
    cout << "\nUndone! Road " << locationName[a] << " <-> " << locationName[b]
         << " is now: ";
    printStatus(old);
    cout << endl;
}
int listA[200];
int listB[200];
int listCount = 0;
void makeRoadList(int onlyProblems)
{
    listCount = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = i + 1; j < N; j++)
        {
            if (road[i][j] != INF)
            {
                if (onlyProblems == 0 || status[i][j] != 0)
                {
                    listA[listCount] = i;
                    listB[listCount] = j;
                    listCount++;
                }
            }
        }
    }
}
void printRoadList()
{
    for (int k = 0; k < listCount; k++)
    {
        int a = listA[k];
        int b = listB[k];
        cout << (k + 1) << ". " << locationName[a] << " <-> " << locationName[b]
             << " | " << road[a][b] << " km | ";
        printStatus(status[a][b]);
        cout << endl;
    }
}
void setRoadStatus(int a, int b, int newStatus)
{
    if (status[a][b] == newStatus)
    {
        cout << "\nThe road already has this status.\n";
        return;
    }
    pushUndo(a, b, status[a][b]);
    status[a][b] = newStatus;
    status[b][a] = newStatus;
    cout << "\nRoad " << locationName[a] << " <-> " << locationName[b]
         << "\nNow: ";
    printStatus(newStatus);
    cout << endl;
}
void viewRoadProblems()
{
    makeRoadList(1);
    cout << "\n ROADS WITH PROBLEMS \n";
    if (listCount == 0)
    {
        cout << "All roads are clear.\n";
        return;
    }
    printRoadList();
}
void reportRoadProblem()
{
    int r, s;
    makeRoadList(0);
    cout << "\n";
    printRoadList();
    cout << "Choose road number (1-" << listCount << "): ";
    cin >> r;
    if (r < 1 || r > listCount)
    {
        cout << "\nWrong road number!\n";
        return;
    }
    cout << "Problem type (1 = Traffic, 2 = Accident, 3 = Construction): ";
    cin >> s;
    if (s < 1 || s > 3)
    {
        cout << "\nWrong problem type!\n";
        return;
    }
    setRoadStatus(listA[r - 1], listB[r - 1], s);
}
void clearRoadProblem()
{
    int r;
    makeRoadList(1);
    if (listCount == 0)
    {
        cout << "\nNo roads to clear. All roads are clear.\n";
        return;
    }
    cout << "\n";
    printRoadList();
    cout << "Choose road number to clear (1-" << listCount << "): ";
    cin >> r;
    if (r < 1 || r > listCount)
    {
        cout << "\nWrong road number!\n";
        return;
    }
    setRoadStatus(listA[r - 1], listB[r - 1], 0);
}
void roadStatusMenu()
{
    int choice = 0;
    while (choice != 5)
    {
        cout << "\n===== ROAD STATUS =====\n";
        cout << "1. Report a road problem\n";
        cout << "2. Clear a road\n";
        cout << "3. View roads with problems\n";
        cout << "4. Undo last change\n";
        cout << "5. Back to main menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1)
        {
            reportRoadProblem();
        }
        else if (choice == 2)
        {
            clearRoadProblem();
        }
        else if (choice == 3)
        {
            viewRoadProblems();
        }
        else if (choice == 4)
        {
            undoLast();
        }
        else if (choice == 5)
        {
            cout << "Going back...\n";
        }
        else
        {
            cout << "Invalid choice! Try again.\n";
        }
    }
}
void viewFacilities(int wanted)
{
    cout << "\n";
    if (wanted == 1)
        cout << "HOSPITALS";
    else if (wanted == 2)
        cout << "FIRE STATIONS";
    else
        cout << "POLICE STATIONS";
    cout << "\n";
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (locationType[i] == wanted)
        {
            count++;
            cout << count << ". " << locationName[i] << endl;
        }
    }
    if (count == 0)
    {
        cout << "None added yet.\n";
    }
    else
    {
        cout << "Total: " << count << endl;
    }
}
void connectNewPlace(int newId)
{
    cout << "\nNow connect it to other places with roads.\n";
    for (int i = 0; i < newId; i++)
    {
        cout << "  " << (i + 1) << " = " << locationName[i] << endl;
    }
    int roadsAdded = 0;
    int other;
    float km;
    while (true)
    {
        cout << "Connect to which place? (enter 0 to finish): ";
        cin >> other;
        if (other == 0)
        {
            if (roadsAdded == 0)
            {
                cout << "Please add at least one road, or the place cannot be reached.\n";
                continue;
            }
            break;
        }
        if (other < 1 || other > newId)
        {
            cout << "Wrong place number!\n";
            continue;
        }
        cout << "Road distance in km (from Google Maps): ";
        cin >> km;
        if (km <= 0)
        {
            cout << "Distance must be more than 0!\n";
            continue;
        }
        addRoad(newId, other - 1, km);
        roadsAdded++;
        cout << "Road added: " << locationName[newId] << " <-> "
             << locationName[other - 1] << " (" << km << " km)\n";
    }
}
void addNewPlace()
{
    if (N >= MAX)
    {
        cout << "\nThe map is full. No more places can be added.\n";
        return;
    }
    int newId = N;
    int t;
    cout << "\nEnter the name of the new place: ";
    cin.ignore();
    cin.getline(locationName[newId], 45);
    cout << "Type (0 = Normal place, 1 = Hospital, 2 = Fire Station, 3 = Police Station): ";
    cin >> t;
    if (t < 0 || t > 3)
    {
        cout << "\nWrong type! Nothing was added.\n";
        return;
    }
    locationType[newId] = t;
    N++;
    connectNewPlace(newId);
    cout << "\nNew place added: " << locationName[newId] << " (";
    printType(locationType[newId]);
    cout << ")\n";
}
void showRoadsOfPlace()
{
    int p;
    cout << "\n";
    for (int i = 0; i < N; i++)
    {
        cout << "  " << (i + 1) << " = " << locationName[i] << endl;
    }
    cout << "Enter place number: ";
    cin >> p;
    p = p - 1;
    if (p < 0 || p >= N)
    {
        cout << "\nWrong place number!\n";
        return;
    }
    cout << "\nRoads of " << locationName[p] << ":\n";
    int count = 0;
    for (int j = 0; j < N; j++)
    {
        if (j != p && road[p][j] != INF)
        {
            count++;
            cout << count << ". to " << locationName[j] << " | " << road[p][j]
                 << " km | ";
            printStatus(status[p][j]);
            cout << endl;
        }
    }
    cout << "Total roads: " << count << endl;
}
void removeLastPlace()
{
    if (N <= REAL_PLACES)
    {
        cout << "\nNo added place to remove. The original places cannot be removed.\n";
        return;
    }
    int last = N - 1;
    cout << "\nRemoved place: " << locationName[last] << endl;
    for (int j = 0; j < MAX; j++)
    {
        road[last][j] = INF;
        road[j][last] = INF;
        status[last][j] = 0;
        status[j][last] = 0;
    }
    road[last][last] = 0;
    N--;
    int k = 0;
    for (int i = 0; i < top; i++)
    {
        if (undoA[i] != last && undoB[i] != last)
        {
            undoA[k] = undoA[i];
            undoB[k] = undoB[i];
            undoOld[k] = undoOld[i];
            k++;
        }
    }
    top = k;
    histCount = 0;
}
void facilitiesMenu()
{
    int choice = 0;
    while (choice != 7)
    {
        cout << "\n===== FACILITIES =====\n";
        cout << "1. View hospitals\n";
        cout << "2. View fire stations\n";
        cout << "3. View police stations\n";
        cout << "4. Add a new place\n";
        cout << "5. View roads of one place\n";
        cout << "6. Remove the last added place\n";
        cout << "7. Back to main menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1)
        {
            viewFacilities(1);
        }
        else if (choice == 2)
        {
            viewFacilities(2);
        }
        else if (choice == 3)
        {
            viewFacilities(3);
        }
        else if (choice == 4)
        {
            addNewPlace();
        }
        else if (choice == 5)
        {
            showRoadsOfPlace();
        }
        else if (choice == 6)
        {
            removeLastPlace();
        }
        else if (choice == 7)
        {
            cout << "Going back...\n";
        }
        else
        {
            cout << "Invalid choice! Try again.\n";
        }
    }
}
int main()
{
    makeMap();
    createRoads();
    int choice = 0;
    while (choice != 6)
    {
       
        cout << "   SMART EMERGENCY RESCUE SYSTEM\n";
        cout << "1. View City Map\n";
        cout << "2. Report Emergency\n";
        cout << "3. Road Status\n";
        cout << "4. Facilities\n";
        cout << "5. Recent Emergencies\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 1)
        {
            showMap();
        }
        else if (choice == 2)
        {
            reportEmergency();
        }
        else if (choice == 3)
        {
            roadStatusMenu();
        }
        else if (choice == 4)
        {
            facilitiesMenu();
        }
        else if (choice == 5)
        {
            showHistory();
        }
        else if (choice == 6)
        {
            cout << "Stay safe. Goodbye!\n";
        }
        else
        {
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
