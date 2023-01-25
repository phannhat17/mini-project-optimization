/*----------------- GUILLOTINE ALGORITHM -----------------*/
#include <bits/stdc++.h>
using namespace std;

const int MAXN  = 1e4 + 7;
const int inf   = INT_MAX;


/*----------------- DECLARATION -----------------*/
int N_items, N_bins;
int bin_used    = 0;
int total_cost  = 0;


/*----------------- BUILD STRUCTURES FOR ITEMS -----------------*/
// Build structure for each item
struct Items
{
    int width, height, area,
        corner_x, corner_y, id;
    bool rotated = 0;
} item[MAXN];

// Build compare function for list of items
bool compare_item_by_longer_side(Items a, Items b)
{
    if(a.height == b.height) return a.width > b.width;
    return a.height > b.height;
}

// Rotating process
void rotate_item(Items &pack)
{
    pack.rotated    = 1 - pack.rotated;
    int temp        = pack.width;
    pack.width      = pack.height;
    pack.height     = temp;
}


/*----------------- BUILD STRUCTURES FOR BINS -----------------*/
//Build structure for list of free_rec in each bin
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

// Build structure for each bin
struct Bins
{
    int width, height,
        area, free_area,
        cost, id;
    vector <Free_Rectangles> list_of_free_rec;
    vector <Items> list_of_items;
} bin[MAXN];

// Build compare function for list of bins
bool compare_bin_by_density(Bins a, Bins b)
{
    // If the density is equal then sort by larger largest side
    if(float(a.cost)/(a.width * a.height) == float(b.cost)/(b.width * b.height))
    {
        if(max(a.width, a.height) == max(b.width, b.height)) return min(a.width, a.height) > min(b.width, b.height);
        return max(a.width, a.height) > max(b.width, b.height);
    }
    // else sort by lower density
    return float(a.cost)/(a.width * a.height) < float(b.cost)/(b.width * b.height);
}


/*----------------- PACKING ITEMS -----------------*/
// Check if the item fit a specific free_rec
bool check_fit_rec(Free_Rectangles rec, Items pack, bool rotated)
{
    // Not rotated case
    if((!rotated) && (pack.width <= rec.width) && (pack.height <= rec.height))
        return 1;
    // Rotated case
    if((rotated) && (pack.width <= rec.height) && (pack.height <= rec.width))
        return 1;

    return 0;
}

// Split initial free_rec after having an insertion
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
// Define spliting method
vector <Free_Rectangles> spliting(Free_Rectangles rec, Items pack)
{
    // Split by Shorter Axis: split on the horizontal axis if rec.width <= rec.height
    return spliting_process(rec.width <= rec.height, rec, pack);
}

// Merge two initial free_recs into a larger free_rec if possible
void merge_rec(Bins &car)
{
    vector <Free_Rectangles> match_width, match_height;

    for(int i = 0; i < car.list_of_free_rec.size(); ++i)
    {
        Free_Rectangles first   = car.list_of_free_rec[i];
        bool check_exist_width  = 0;
        bool check_exist_height = 0;
        int pos_check_width     = 0;
        int pos_check_height    = 0;
        // Loop to find a mergable free_rec for the rec i
        for(int j = 0; j < car.list_of_free_rec.size(); ++j)
        {
            Free_Rectangles second  = car.list_of_free_rec[j];
            if(j == i) continue;
            // Find the mergable free_rec with the same width
            if((first.width == second.width) && (first.corner_x == second.corner_x) && (second.corner_y == first.corner_y + first.height))
            {
                check_exist_width   = 1;
                pos_check_width     = j;
                break;
            }
            // Find the mergable free_rec with the same height
            if((first.height == second.height) && (first.corner_y == second.corner_y) && (second.corner_x == first.corner_x + first.width))
            {
                check_exist_height  = 1;
                pos_check_height    = j;
                break;
            }
        }
        // Merge two free_rec with the same width
        if(check_exist_width)
        {
            Free_Rectangles merged_rec;
            merged_rec.width    = first.width;
            merged_rec.height   = first.height + car.list_of_free_rec[pos_check_width].height;
            merged_rec.area     = merged_rec.width * merged_rec.height;
            merged_rec.corner_x = first.corner_x;
            merged_rec.corner_y = first.corner_y;
            // Remove the two initial free_recs
            car.list_of_free_rec.erase(car.list_of_free_rec.begin() + pos_check_width);
            if(pos_check_width < i) --i;
            car.list_of_free_rec.erase(car.list_of_free_rec.begin() + i), --i;
            // Add the merged free_rec
            car.list_of_free_rec.push_back(merged_rec);
        }

        // Merge two free_rec with the same height
        if(check_exist_height)
        {
            Free_Rectangles merged_rec;
            merged_rec.width    = first.width + car.list_of_free_rec[pos_check_height].width;
            merged_rec.height   = first.height;
            merged_rec.area     = merged_rec.width * merged_rec.height;
            merged_rec.corner_x = first.corner_x;
            merged_rec.corner_y = first.corner_y;
            // Remove the two initial free_recs
            car.list_of_free_rec.erase(car.list_of_free_rec.begin() + pos_check_height);
            if(pos_check_height < i) --i;
            car.list_of_free_rec.erase(car.list_of_free_rec.begin() + i), --i;
            // Add the merged free_rec
            car.list_of_free_rec.push_back(merged_rec);
        }
    }
}

// Adding item into a bin at the bottom-left-corner of a free_rec
void add_item(Bins &car, Items &pack, bool rotated, int x, int y)
{
    if(rotated) rotate_item(pack);
    pack.corner_x = x;
    pack.corner_y = y;
    car.list_of_items.push_back(pack);
    car.free_area -= pack.area;
}

// Build compare function for ranking
bool compare_ranking_rec_BSS(pair <int, int> a, pair <int, int> b)
{
    if(a.first == b.first) return a.second > b.second;
    return a.first < b.first;
}
// Calculate score for the item when insert to a free_rec
pair <int, int> score_rec(Free_Rectangles rec, Items pack, bool rotated)
{
    pair <int, int> score;

    // Best Short Side: shorter reaminder side after insertion is minimized, ties broken with best long
    // Rotate case
    if(rotated)
    {
        score.first     = min(rec.width - pack.height, rec.height - pack.width);
        score.second    = max(rec.width - pack.height, rec.height - pack.width);
    }
    // Not rotate case
    else
    {
        score.first     = min(rec.width - pack.width, rec.height - pack.height);
        score.second    = max(rec.width - pack.width, rec.height - pack.height);
    }

    return score;
}
// Find best free_rec for the item in the list free_rec of a bin
pair <pair <Free_Rectangles, int>, pair <bool, bool>> best_ranking(Bins car, Items pack)
{
    bool rotated;
    Free_Rectangles best_rec;
    int best_pos, check_exist = 0;
    pair <int, int> best_score;
    pair <pair <Free_Rectangles, int>, pair <bool, bool>> ans_return;
    best_score.first = best_score.second = inf;

    // Loop to find the best score
    for(int i = 0; i < car.list_of_free_rec.size(); ++i)
    {
        Free_Rectangles rec = car.list_of_free_rec[i];
        // Not rotate case
        if(check_fit_rec(rec, pack, 0) && compare_ranking_rec_BSS(score_rec(rec, pack, 0), best_score))
        {
            best_score  = score_rec(rec, pack, 0);
            best_rec    = rec;
            best_pos    = i;
            rotated     = 0;
            check_exist = 1;
        }
        // Rotate case
        if(check_fit_rec(rec, pack, 1) && compare_ranking_rec_BSS(score_rec(rec, pack, 1), best_score))
        {
            best_score  = score_rec(rec, pack, 1);
            best_rec    = rec;
            best_pos    = i;
            rotated     = 1;
            check_exist = 1;
        }
    }

    //Prepare for returning value
    ans_return.first.first      = best_rec;
    ans_return.first.second     = best_pos;
    ans_return.second.first     = rotated;
    ans_return.second.second    = check_exist;

    return ans_return;
}

// Check if item find a possible free_rec in the bin for inserting process
bool insert_item(Bins &car, Items &pack)
{
    pair <pair <Free_Rectangles, int>, pair <bool, bool>> best_ranking_return;
    best_ranking_return         = best_ranking(car, pack);

    // If the free_rec which fits the item does not exist
    if(!best_ranking_return.second.second) return 0;

    // If the free_rec exists
    Free_Rectangles best_rec    = best_ranking_return.first.first;
    int best_pos                = best_ranking_return.first.second;
    int rotated                 = best_ranking_return.second.first;
    // Add the item into the choosen free_rec
    add_item(car, pack, rotated, best_rec.corner_x, best_rec.corner_y);
    // Replace the used free_rec by the new splited rec(s)
    car.list_of_free_rec.erase(car.list_of_free_rec.begin() + best_pos);
    vector <Free_Rectangles> new_rec = spliting(best_rec, pack);
    for(auto rec : new_rec)
    {
        car.list_of_free_rec.push_back(rec);
    }
    // Merge the free_rec if possible
    merge_rec(car);

    return 1;
}


/*----------------- READ INPUT -----------------*/
void Enter()
{
    cin >> N_items >> N_bins;
    // Read and prepare for the list of items
    for(int i = 1; i <= N_items; ++i)
    {
        cin >> item[i].width >> item[i].height;
        if(item[i].width > item[i].height) rotate_item(item[i]);
        item[i].area    = item[i].width * item[i].height;
        item[i].id      = i;
    }

    // Read and prepare for the list of bins
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
void Solve()
{
    sort(item + 1, item + N_items + 1, compare_item_by_longer_side);
    sort(bin + 1, bin + N_bins + 1, compare_bin_by_density);
    for(int i = 1; i <= N_items; ++i)
    {
        // Bin First Fit: choose bin that first fit
        for(int j = 1; j <= N_bins; ++j)
            if(insert_item(bin[j],item[i])) break;
    }
}


/*----------------- PRINT OUTPUT -----------------*/
void Print()
{
    // Calculate total number of used bin and total cost
    for(int j = 1; j <= N_bins; ++j)
    {
        if(bin[j].list_of_items.size() > 0)
        {
            total_cost += bin[j].cost;
            ++bin_used;
        }
    }

    // Print the solution
    cout << "Number of item given: "    << N_items      << endl;
    cout << "Number of bin given: "     << N_bins       << endl;
    cout << "Number of bin used: "      << bin_used     << endl;
    cout << "Total cost: "              << total_cost   << endl;

    // Uncomment for more detail information of packed items in each used bin
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
signed main(int argv, char* argc[])
{
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    if(argv < 2)
    {
        cout << "Error: No file name provided." << endl;
        return 0;
    }
    freopen(argc[1], "r", stdin);

    // Start timing the process
    clock_t start_timing = clock();

    // Run solver
    Enter();
    Solve();
    Print();

    // Print additional informations
    cout << "Status: "                  << "None"       << endl;
    cout << "Time limit: "              << "None"       << endl;
    cout << "Running time: ";
    cout.precision(20);
    cout << float(clock() - start_timing)/CLOCKS_PER_SEC << endl;
}
