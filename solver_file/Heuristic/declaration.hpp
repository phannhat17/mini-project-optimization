#include <bits/stdc++.h>

using namespace std;

const int MAXN  = 1e4 + 7;
const int inf   = INT_MAX;


/*----------------- DECLARATION -----------------*/
int N_items, N_bins;
int bin_used, total_cost;
bool check_algorithm = 0;

/*----------------- BUILD STRUCTURES FOR ITEMS -----------------*/
// Build structure for each item
struct Items
{
    int width, height, area,
        corner_x, corner_y, id;
    int pos_bin;
    bool rotated = 0;
} item[MAXN], item_guillotine[MAXN];

// Build compare function for list of items
bool compare_item_by_longer_side(Items a, Items b)
{
    if(a.height == b.height) return a.width > b.width;
    return a.height > b.height;
}

// Build compare function for list item to return to the order of the input list
bool compare_reset_item(Items a, Items b)
{
    return a.id < b.id;
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


/*----------------- GENERAL PACKING ITEMS FUNCTIONS -----------------*/
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
    if(a.first == b.first) return a.second < b.second;
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
pair <pair <Free_Rectangles, unsigned int>, pair <bool, bool>> best_ranking(Bins car, Items pack)
{
    bool rotated = 0;
    Free_Rectangles best_rec;
    unsigned int best_pos = 0;
    int check_exist = 0;
    pair <int, int> best_score;
    pair <pair <Free_Rectangles, unsigned int>, pair <bool, bool>> ans_return;
    best_score.first = best_score.second = inf;

    // Loop to find the best score
    for(unsigned int i = 0; i < car.list_of_free_rec.size(); ++i)
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


/*----------------- PREPARE SOLUTION -----------------*/
pair <int, int> calculate_solution()
{
    pair <int, int> return_solution;
    total_cost = 0; bin_used = 0;

    // Calculate total number of used bin and total cost
    for(int j = 1; j <= N_bins; ++j)
    {
        if(bin[j].list_of_items.size() > 0)
        {
            total_cost += bin[j].cost;
            ++bin_used;
        }
    }

    return_solution.first   = total_cost;
    return_solution.second  = bin_used;

    return return_solution;
}


/*----------------- CHECKING ITEMS AND BINS STATUS ------------------*/
void checking_status(bool algorithm)
{
//    cout << "/*----------------- These are the items -----------------*/" << '\n';
//    for(int i = 1; i <= N_items; ++i)
//    {
//        Items pack = item[i];
//        cout    << "ID: " << pack.id << ", Width: " << pack.width << ", Height: " << pack.height << ", "
//                << "Corner x: " << pack.corner_x << ", Corner y: " << pack.corner_y << ", Rotated: " << pack.rotated << '\n';
//    }
//    cout << "/*----------------- These are the bins -----------------*/" << '\n';

    // Debugging format: just one algorithm at a time
//    for(int j = 1; j <= N_bins; ++j)
//    {
//        if(bin[j].list_of_items.size() == 0)
//        {
//            continue;
//        }
//        cout << "/*----------------- Consider Bin: ------------------*/" << '\n';
//        cout << "Bin id: " << bin[j].id << '\n';
//        cout << "Bin width: " << bin[j].width << '\n';
//        cout << "Bin height: " << bin[j].height << '\n';
//        cout << "Bin cost: " << bin[j].cost << '\n';
//        cout << "Bin area: " << bin[j].area << '\n';
//        cout << "Bin remain area: " << bin[j].free_area << '\n';
//        cout << "/*----------------- List of items ------------------*/" << '\n';
//        for(auto pack : bin[j].list_of_items)
//        {
//            cout    << "ID: " << pack.id << ", Width: " << pack.width << ", Height: " << pack.height << ", "
//                    << "Corner x: " << pack.corner_x << ", Corner y: " << pack.corner_y << ", Rotated: " << pack.rotated << '\n';
//        }
//        cout << '\n' << '\n';
//    }

    // Output format for claiming data
    if(algorithm == 0)
    {
        for(int i = 1; i <= N_items; ++i)
        {
            if(item_guillotine[i].rotated)      cout << "Rotate pack "  << item_guillotine[i].id << " and put";
            else                                cout << "Put pack "     << item_guillotine[i].id;
            cout    << " in bin " << item_guillotine[i].pos_bin
                    << " that the top right corner coordinate (x, y) is ("
                    << item_guillotine[i].corner_x + item_guillotine[i].width << ", " << item_guillotine[i].corner_y + item_guillotine[i].height << ")" << '\n';
        }
    }
    else
    {
        for(int i = 1; i <= N_items; ++i)
        {
            if(item[i].rotated)     cout << "Rotate pack "  << item[i].id << " and put";
            else                    cout << "Put pack "     << item[i].id;
            cout    << " in bin " << item[i].pos_bin
                    << " that the top right corner coordinate (x, y) is ("
                    << item[i].corner_x + item[i].width << ", " << item[i].corner_y + item[i].height << ")" << '\n';
        }
    }

    // Output format for submitting
//    if(algorithm == 0)
//    {
//        sort(item_guillotine + 1, item_guillotine + N_items + 1, compare_reset_item);
//        for(int i = 1; i <= N_items; ++i)
//        {
//            cout    << item_guillotine[i].id        << ' ' << item_guillotine[i].pos_bin  << ' '
//                    << item_guillotine[i].corner_x  << ' ' << item_guillotine[i].corner_y << ' '
//                    << item_guillotine[i].rotated   << '\n';
//        }
//    }
//    else
//    {
//        sort(item + 1, item + N_items + 1, compare_reset_item);
//        for(int i = 1; i <= N_items; ++i)
//        {
//            cout    << item[i].id        << ' ' << item[i].pos_bin  << ' '
//                    << item[i].corner_x  << ' ' << item[i].corner_y << ' '
//                    << item[i].rotated   << '\n';
//        }
//    }
}
