#include <bits/stdc++.h>
using namespace std;
//using namespace std::chrono;

const int MAXN  = 1e4 + 7;
const int inf   = INT_MAX;

/*----------------- DECLARATION -----------------*/
int N_items, N_bins;
int bin_used    = 0;
int total_cost  = 0;

/*----------------- BUILD STRUCTURES FOR ITEMS -----------------*/
struct Items
{
    int width, height, area,
        corner_x, corner_y, id;
    bool rotated = 0;
} item[MAXN];

bool compare_item_by_longer_side(Items a, Items b)
{
    if(a.height == b.height) return a.width > b.width;
    return a.height > b.height;
}

void rotate_item(Items &pack)
{
//    cout << "First rotate: " << pack.rotated << endl;
    pack.rotated    = 1 - pack.rotated;
//    cout << "Second rotate: " << pack.rotated << endl;
    int temp        = pack.width;
    pack.width      = pack.height;
    pack.height     = temp;
}

/*----------------- BUILD STRUCTURES FOR BINS -----------------*/
struct Free_Rectangles
{
    int corner_x, corner_y,
        width, height, area;
    bool operator == (const Free_Rectangles &x) const
    {
        return  (corner_x   == x.corner_x)    &&
                (corner_y   == x.corner_y)    &&
                (width      == x.width)       &&
                (height     == x.height);
    }
};
struct Bins
{
    int width, height,
        area, free_area,
        cost, id;
    vector <Free_Rectangles> list_of_free_rec;
    vector <Items> list_of_items;
} bin[MAXN];

bool compare_bin_by_density(Bins a, Bins b)
{
    if(float(a.cost)/(a.width * a.height) == float(b.cost)/(b.width * b.height))
    {
        if(max(a.width, a.height) == max(b.width, b.height)) return min(a.width, a.height) > min(b.width, b.height);
        return max(a.width, a.height) > max(b.width, b.height);
    }
    return float(a.cost)/(a.width * a.height) < float(b.cost)/(b.width * b.height);
}

bool compare_bin_by_cost(Bins a, Bins b)
{
    if(a.cost == b.cost)
    {
        if(max(a.width, a.height) == max(b.width, b.height)) return min(a.width, a.height) > min(b.width, b.height);
        return max(a.width, a.height) > max(b.width, b.height);
    }
    return a.cost < b.cost;
}
/*----------------- PACKING ITEMS -----------------*/
bool check_fit_rec(Free_Rectangles rec, Items pack, bool rotated)
{
    if((!rotated) && (pack.width <= rec.width) && (pack.height <= rec.height))
        return 1;
    if((rotated) && (pack.width <= rec.height) && (pack.height <= rec.width))
        return 1;
    return 0;
}

vector <Free_Rectangles> spliting_process(bool horizontal, Free_Rectangles rec, Items pack)
{
    vector <Free_Rectangles> list_of_free_rec;
    Free_Rectangles new_free_rec;

    int right_x     = rec.corner_x + pack.width,
        right_y     = rec.corner_y,
        right_width = rec.width - pack.width,
        top_x       = rec.corner_x,
        top_y       = rec.corner_y + pack.height,
        top_height  = rec.height - pack.height;

    // horizontal split if split == 1, otherwise vertical split
    int right_height    = (horizontal)? pack.height : rec.height;
    int top_width       = (horizontal)? rec.width   : pack.width;
    if((right_width > 0) & (right_height > 0))
    {
        new_free_rec.corner_x   = right_x;
        new_free_rec.corner_y   = right_y;
        new_free_rec.width      = right_width;
        new_free_rec.height     = right_height;
        list_of_free_rec.push_back(new_free_rec);
    }
    if((top_width > 0) & (top_height > 0))
    {
        new_free_rec.corner_x   = top_x;
        new_free_rec.corner_y   = top_y;
        new_free_rec.width      = top_width;
        new_free_rec.height     = top_height;
        list_of_free_rec.push_back(new_free_rec);
    }
    return list_of_free_rec;
}

vector <Free_Rectangles> spliting(Free_Rectangles rec, Items pack)
{
    // Split by Shorter Axis: split on the horizontal axis if rec.width <= rec.height
    return spliting_process(rec.width <= rec.height, rec, pack);
}

void merge_rec(Bins &car)
{
    vector <Free_Rectangles> match_width, match_height;
    for(int i = 0; i < car.list_of_free_rec.size(); ++i)
    {
        Free_Rectangles first   = car.list_of_free_rec[i];
        bool check_exist_width  = 0;
        bool check_exist_height = 0;
        int pos_check_width, pos_check_height;
        for(int j = 0; j < car.list_of_free_rec.size(); ++j)
        {
            Free_Rectangles second  = car.list_of_free_rec[j];
            if(j == i) continue;
            if((first.width == second.width) && (first.corner_x == second.corner_x) && (second.corner_y == first.corner_y + first.height))
            {
                check_exist_width   = 1;
                pos_check_width     = j;
                break;
            }
            if((first.height == second.height) && (first.corner_y == second.corner_y) && (second.corner_x == first.corner_x + first.width))
            {
                check_exist_height  = 1;
                pos_check_height    = j;
            }
        }
        if(check_exist_width)
        {
            Free_Rectangles merged_rec;
            merged_rec.width    = first.width;
            merged_rec.height   = first.height + car.list_of_free_rec[pos_check_width].height;
            merged_rec.area     = merged_rec.width * merged_rec.height;
            merged_rec.corner_x = first.corner_x;
            merged_rec.corner_y = first.corner_y;
            car.list_of_free_rec.erase(car.list_of_free_rec.begin() + pos_check_width);
            if(pos_check_width < i) --i;
            car.list_of_free_rec.erase(car.list_of_free_rec.begin() + i), --i;
            car.list_of_free_rec.push_back(merged_rec);
        }
        if(check_exist_height)
        {
            Free_Rectangles merged_rec;
            merged_rec.width    = first.width + car.list_of_free_rec[pos_check_height].width;
            merged_rec.height   = first.height;
            merged_rec.area     = merged_rec.width * merged_rec.height;
            merged_rec.corner_x = first.corner_x;
            merged_rec.corner_y = first.corner_y;
            car.list_of_free_rec.erase(car.list_of_free_rec.begin() + pos_check_height);
            if(pos_check_width < i) --i;
            car.list_of_free_rec.erase(car.list_of_free_rec.begin() + i), --i;
            car.list_of_free_rec.push_back(merged_rec);
        }
    }
}

void add_item(Bins &car, Items &pack, bool rotated, int x, int y)
{
    if(rotated) rotate_item(pack);
    pack.corner_x = x;
    pack.corner_y = y;
    car.list_of_items.push_back(pack);
    car.free_area -= pack.area;
}

bool compare_ranking_rec_BSS(pair <int, int> a, pair <int, int> b)
{
    if(a.first == b.first) return a.second > b.second;
    return a.first < b.first;
}

pair <int, int> score_rec(Free_Rectangles rec, Items pack, bool rotated)
{
    pair <int, int> score;
    // Best Short Side: shorter reaminder side after insertion is minimized, ties broken with best long
    if(rotated)
    {
        score.first     = min(rec.width - pack.height, rec.height - pack.width);
        score.second    = max(rec.width - pack.height, rec.height - pack.width);
    }
    else
    {
        score.first     = min(rec.width - pack.width, rec.height - pack.height);
        score.second    = max(rec.width - pack.width, rec.height - pack.height);
    }
    return score;
}

pair <pair <Free_Rectangles, int>, pair <bool, bool>> best_ranking(Bins car, Items pack)
{
    bool rotated;
    Free_Rectangles best_rec;
    int best_pos;
    int check_exist = 0;
    pair <int, int> best_score;
    pair <pair <Free_Rectangles, int>, pair <bool, bool>> ans_return;
    best_score.first = best_score.second = inf;
    for(int i = 0; i < car.list_of_free_rec.size(); ++i)
    {
        Free_Rectangles rec = car.list_of_free_rec[i];
        if(check_fit_rec(rec, pack, 0) && compare_ranking_rec_BSS(score_rec(rec, pack, 0), best_score))
        {
            best_score  = score_rec(rec, pack, 0);
            best_rec    = rec;
            best_pos    = i;
            rotated     = 0;
            check_exist = 1;
//            cout << "Hmm " << pack.id << ' ' << best_score.first << ' ' << rotated << endl;
        }
        if(check_fit_rec(rec, pack, 1) && compare_ranking_rec_BSS(score_rec(rec, pack, 1), best_score))
        {
            best_score  = score_rec(rec, pack, 1);
            best_rec    = rec;
            best_pos    = i;
            rotated     = 1;
            check_exist = 1;
//            cout << "Hmm " << pack.id << ' ' << best_score.first << ' ' << rotated << endl;
        }
    }
    ans_return.first.first      = best_rec;
    ans_return.first.second     = best_pos;
    ans_return.second.first     = rotated;
    ans_return.second.second    = check_exist;
    return ans_return;
}

bool insert_item(Bins &car, Items &pack)
{
    pair <pair <Free_Rectangles, int>, pair <bool, bool>> best_ranking_return;
    best_ranking_return         = best_ranking(car, pack);
//    cout << "/*----------------- Consider item " << pack.id << " and bin " << car.id << " ------------------*/" << endl;
    // If the free_rec which fits the item does not exist
    if(!best_ranking_return.second.second) return 0;

    // If the free_rec exists
    Free_Rectangles best_rec    = best_ranking_return.first.first;
    int best_pos                = best_ranking_return.first.second;
    bool rotated                = best_ranking_return.second.first;
//    cout << "/*----------------- Checking rotated -----------------*/" << endl;
//    cout << rotated << endl;
//    cout << pack.rotated << ' ' << pack.width << ' ' << pack.height<< endl;
    //if(rotated) rotate_item(pack);
    add_item(car, pack, rotated, best_rec.corner_x, best_rec.corner_y);
//    cout << pack.rotated << ' ' << pack.width << ' ' << pack.height<< endl;
    // Replace the huge free_rec with the splited rec(s)
//    cout << "Initial list: ";
//    for(auto rec : car.list_of_free_rec)
//    {
//        cout << "(" << rec.width << ", " << rec.height << ", " << rec.corner_x << ", " << rec.corner_y << ")" << ' ';
//    }
//    cout << endl;
//    cout << "Best rec: ";
//    cout << "(" << best_rec.width << ", " << best_rec.height << ", " << best_rec.corner_x << ", " << best_rec.corner_y << ")" << endl;
    car.list_of_free_rec.erase(car.list_of_free_rec.begin() + best_pos);
    vector <Free_Rectangles> new_rec = spliting(best_rec, pack);
//    cout << "New rec: ";
    for(auto rec : new_rec)
    {
//        cout << "(" << rec.width << ", " << rec.height << ", " << rec.corner_x << ", " << rec.corner_y << ")" << ' ';
        car.list_of_free_rec.push_back(rec);
    }
//    cout << endl;
//    cout << "New list: ";
//    for(auto rec : car.list_of_free_rec)
//    {
//        cout << "(" << rec.width << ", " << rec.height << ", " << rec.corner_x << ", " << rec.corner_y << ")"<< ' ';
//    }
//    cout<<endl;
    merge_rec(car);
    return 1;
}


/*----------------- READ INPUT -----------------*/
void Enter()
{
    cin >> N_items >> N_bins;

    for(int i = 1; i <= N_items; ++i)
    {
        cin >> item[i].width >> item[i].height;
        if(item[i].width > item[i].height) rotate_item(item[i]);
        item[i].area    = item[i].width * item[i].height;
        item[i].id      = i;
    }

    for(int j = 1; j <= N_bins; ++j)
    {
        cin >> bin[j].width >> bin[j].height >> bin[j].cost;
        bin[j].area = bin[j].width * bin[j].height; bin[j].id = j;
        bin[j].free_area    = bin[j].area;
        Free_Rectangles first_rec;
        first_rec.width     = bin[j].width;
        first_rec.height    = bin[j].height;
        first_rec.corner_x  = first_rec.corner_y = 0;
        first_rec.area      = first_rec.width * first_rec.height;
        bin[j].list_of_free_rec.push_back(first_rec);
    }
}

/*----------------- BIN CHOOSING -----------------*/
void Solve_sort_by_density()
{
    sort(item + 1, item + N_items + 1, compare_item_by_longer_side);
    sort(bin + 1, bin + N_bins + 1, compare_bin_by_density);
    for(int i = 1; i <= N_items; ++i)
    {
        // Choose Bin First Fit
        for(int j = 1; j <= N_bins; ++j)
        {
            if(insert_item(bin[j],item[i]))
            {
                break;
            }
        }
    }
}

void ResetBin()
{
    for(int j = 1; j <= N_bins; ++j)
    {
        bin[j].free_area    = bin[j].area;
        bin[j].list_of_free_rec.clear();
        Free_Rectangles first_rec;
        first_rec.width     = bin[j].width;
        first_rec.height    = bin[j].height;
        first_rec.corner_x  = first_rec.corner_y = 0;
        first_rec.area      = first_rec.width * first_rec.height;
        bin[j].list_of_free_rec.push_back(first_rec);
    }
    total_cost = 0;
}

void Solve_sort_by_cost()
{
//    sort(item + 1, item + N_items + 1, compare_item_by_longer_side);
    sort(bin + 1, bin + N_bins + 1, compare_bin_by_cost);
    for(int i = 1; i <= N_items; ++i)
    {
        // Choose Bin First Fit
        for(int j = 1; j <= N_bins; ++j)
        {
            if(insert_item(bin[j],item[i]))
            {
                break;
            }
        }
    }
}
void Print()
{
    for(int j = 1; j <= N_bins; ++j)
    {
        if(bin[j].list_of_items.size() > 0)
        {
            total_cost += bin[j].cost;
            ++bin_used;
        }
    }
    cout << "Number of item given: "    << N_items      << endl;
    cout << "Number of bin given: "     << N_bins       << endl;
    cout << "Number of bin used: "      << bin_used     << endl;
    cout << "Total cost: "              << total_cost   << endl;
//    cout << "/*----------------- These are the items -----------------*/" << endl;
//    for(int i = 1; i <= N_items; ++i)
//    {
//        if(checking_rotate_function(item[i], 1)) cout << "Aizzz ";
//        cout << item[i].width << ' ' << item[i].height << ' ' << item[i].area << ' ' << item[i].rotated << endl;
//    }
//    cout << "/*----------------- These are the bins -----------------*/" << endl;
//    for(int j = 1; j <= N_bins; ++j)
//    {
//        cout << endl;
//        if(bin[j].list_of_items.size() == 0)
//        {
//            continue;
//        }
//        cout << "/*----------------- Consider Bin: ------------------*/" << endl;
//        cout << "Bin id: " << bin[j].id << endl;
//        cout << "Bin width: " << bin[j].width << endl;
//        cout << "Bin height: " << bin[j].height << endl;
//        cout << "Bin cost: " << bin[j].cost <<endl;
//        cout << "/*----------------- List of items ------------------*/" << endl;
//        for(auto k:bin[j].list_of_items)
//        {
//            cout << k.id << ' ' << k.width << ' ' << k.height << ' ' << k.corner_x << ' ' << k.corner_y << ' ' << k.rotated << endl;
//        }
//        cout << endl;
//    }
}

/*----------------- MAIN -----------------*/
signed main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);

    // Check if a file name was passed as an argument
    if (argc < 2) {
        cout << "Error: No file name provided." << endl;
        return 1;
    }

    // Open the file for reading
    freopen(argv[1], "r", stdin);

//    freopen("testing.out", "w", stdout);
//    auto start_point    = high_resolution_clock::now();
    clock_t start_timing = clock();
    Enter();
    Solve_sort_by_density();
    Print();
//    ResetBin();
//    Solve_sort_by_cost();
//    Print();
//    auto end_point  = high_resolution_clock::now();
//    auto duration   = duration_cast<nanoseconds>(end_point - start_point);
//    cout << duration.count() << " ns";
    cout << "Status: "                  << "None"       << endl;
    cout << "Time limit: "              << "None"       << endl;
    cout << "Running Time: ";
    cout.precision(20);
    cout << float(clock() - start_timing)/CLOCKS_PER_SEC;
}