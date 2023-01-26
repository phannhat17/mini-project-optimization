
/*----------------- MAXIMAL RECTANGLES ALGORITHM -----------------*/
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

bool compare_bin_by_costs(Bins a, Bins b)
{
    if(a.cost == b.cost)
    {
        if(max(a.width, a.height) == max(b.width, b.height)) return min(a.width, a.height) > min(b.width, b.height);
        return max(a.width, a.height) > max(b.width, b.height);
    }
    return a.cost < b.cost;
}
/*----------------- PACKING ITEMS -----------------*/
// Check if the item fit a specific free_rec => The same as guillotine
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
vector <Free_Rectangles> spliting_process_maxrec(Free_Rectangles rec, Items pack)
{
    vector <Free_Rectangles> list_of_free_rec;
    Free_Rectangles new_free_rec;

    // Vertical split to maximize the right corner free_rec
    if(pack.width < rec.width)
    {
        new_free_rec.width      = rec.width - pack.width;
        new_free_rec.height     = rec.height;
        new_free_rec.corner_x   = rec.corner_x + pack.width;
        new_free_rec.corner_y   = rec.corner_y;
        list_of_free_rec.push_back(new_free_rec);
    }

    // Horizontal split to maximize the top corner free_rec
    if(pack.height < rec.height)
    {
        new_free_rec.width      = rec.width;
        new_free_rec.height     = rec.height - pack.height;
        new_free_rec.corner_x   = rec.corner_x;
        new_free_rec.corner_y   = rec.corner_y + pack.height;
        list_of_free_rec.push_back(new_free_rec);
    }

    return list_of_free_rec;
}

// Check if the item intersects other free_rec
bool check_intersec(Free_Rectangles rec, Items pack)
{
    if(pack.corner_x >= rec.corner_x + rec.width)   return 0;
    if(pack.corner_y >= rec.corner_y + rec.height)  return 0;
    if(pack.corner_x + pack.width <= rec.corner_x)  return 0;
    if(pack.corner_y + pack.height <= rec.corner_y) return 0;
    return 1;
}

// Find overlap part
Free_Rectangles find_overlap(Free_Rectangles rec, Items pack)
{
    Free_Rectangles overlap_rec;
    overlap_rec.corner_x    = max(rec.corner_x, pack.corner_x);
    overlap_rec.corner_y    = max(rec.corner_y, pack.corner_y);
    overlap_rec.width       = min(rec.corner_x + rec.width, pack.corner_x + pack.width) - overlap_rec.corner_x;
    overlap_rec.height      = min(rec.corner_y + rec.height, pack.corner_y + pack.height) - overlap_rec.corner_y;
    return overlap_rec;
}

// Split the intersected rec into smaller free_recs
vector <Free_Rectangles> split_intersect (Free_Rectangles initial_rec, Free_Rectangles overlap_rec)
{
    vector <Free_Rectangles> list_of_free_rec;
    Free_Rectangles new_free_rec;
    // Vertical split to maximize the left corner free_rec
    if(overlap_rec.corner_x > initial_rec.corner_x)
    {
        new_free_rec.corner_x   = initial_rec.corner_x;
        new_free_rec.corner_y   = initial_rec.corner_y;
        new_free_rec.width      = overlap_rec.corner_x - new_free_rec.corner_x;
        new_free_rec.height     = initial_rec.height;
        list_of_free_rec.push_back(new_free_rec);
    }

    // Vertical split to maximize the right corner free_rec
    if(overlap_rec.corner_x + overlap_rec.width < initial_rec.corner_x + initial_rec.width)
    {
        new_free_rec.corner_x   = overlap_rec.corner_x + overlap_rec.width;
        new_free_rec.corner_y   = initial_rec.corner_y;
        new_free_rec.width      = initial_rec.corner_x + initial_rec.width - new_free_rec.corner_x;
        new_free_rec.height     = initial_rec.height;
        list_of_free_rec.push_back(new_free_rec);
    }

    // Horizontal split to maximize the bottom corner free_rec
    if(overlap_rec.corner_y > initial_rec.corner_y)
    {
        new_free_rec.corner_x   = initial_rec.corner_x;
        new_free_rec.corner_y   = initial_rec.corner_y;
        new_free_rec.width      = initial_rec.width;
        new_free_rec.height     = overlap_rec.corner_y - new_free_rec.corner_y;
        list_of_free_rec.push_back(new_free_rec);
    }

    // Horizontal split to maximize the top corner free_rec
    if(overlap_rec.corner_y + overlap_rec.height < initial_rec.corner_y + initial_rec.height)
    {
        new_free_rec.corner_x   = initial_rec.corner_x;
        new_free_rec.corner_y   = overlap_rec.corner_y + overlap_rec.height;
        new_free_rec.width      = initial_rec.width;
        new_free_rec.height     = initial_rec.corner_y + initial_rec.height - new_free_rec.corner_y;
        list_of_free_rec.push_back(new_free_rec);
    }

    return list_of_free_rec;
}

// Check if a free_rec is fully covered by another free_rec
bool check_covered(Free_Rectangles rec_covering, Free_Rectangles rec_covered)
{
    // Not intersect
    if(rec_covered.corner_x > rec_covering.corner_x + rec_covering.width)   return 0;
    if(rec_covered.corner_y > rec_covering.corner_y + rec_covering.height)  return 0;
    if(rec_covered.corner_x + rec_covered.width < rec_covering.corner_x)    return 0;
    if(rec_covered.corner_y + rec_covered.height < rec_covering.corner_y)   return 0;

    // Intersect but not fully covered
    if(rec_covered.corner_x < rec_covering.corner_x) return 0;
    if(rec_covered.corner_y < rec_covering.corner_y) return 0;
    if(rec_covered.corner_x + rec_covered.width > rec_covering.corner_x + rec_covering.width)   return 0;
    if(rec_covered.corner_y + rec_covered.height > rec_covering.corner_y + rec_covering.height) return 0;

    return 1;
}

// Remove the covered free_rec
void remove_covered_rec(Bins &car)
{
    for(int i = 0; i < car.list_of_free_rec.size(); ++i)
    {
        Free_Rectangles first = car.list_of_free_rec[i];
        for(int j = i + 1; j < car.list_of_free_rec.size(); ++j)
        {
            Free_Rectangles second = car.list_of_free_rec[j];
            // If rec i cover rec j then delete rec j
            if(check_covered(first, second))
            {
                car.list_of_free_rec.erase(car.list_of_free_rec.begin() + j);
                --j;
                continue;
            }
            // If rec j cover rec i then delete rec i
            if(check_covered(second, first))
            {
                car.list_of_free_rec.erase(car.list_of_free_rec.begin() + i);
                --i;
                break;
            }
        }
    }
}

// Remove overlap part
void remove_overlap(Bins &car, Items pack)
{
    for(int i = 0; i < car.list_of_free_rec.size(); ++i)
    {
        Free_Rectangles rec = car.list_of_free_rec[i];
        if(check_intersec(rec, pack))
        {
            Free_Rectangles overlap_rec = find_overlap(rec, pack);
            vector <Free_Rectangles> new_rec = split_intersect(rec, overlap_rec);
            car.list_of_free_rec.erase(car.list_of_free_rec.begin() + i);
            for(auto free_rec : new_rec)
            {
                car.list_of_free_rec.push_back(free_rec);
            }
            --i;
        }
    }
    remove_covered_rec(car);
}

// Adding item into a bin at the bottom-left-corner of a free_rec => The same as guillotine
void add_item(Bins &car, Items &pack, bool rotated, int x, int y)
{
    if(rotated) rotate_item(pack);
    pack.corner_x = x;
    pack.corner_y = y;
    car.list_of_items.push_back(pack);
    car.free_area -= pack.area;
}

// Build compare function for ranking => Same technique as guillotine
bool compare_ranking_rec_BSS(pair <int, int> a, pair <int, int> b)
{
    if(a.first == b.first) return a.second > b.second;
    return a.first < b.first;
}
// Calculate score for the item when insert to a free_rec => Same technique as guillotine
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
// Find best free_rec for the item in the list free_rec of a bin => Same technique as guillotine
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
bool insert_item_maxrec(Bins &car, Items &pack)
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
    vector <Free_Rectangles> new_rec = spliting_process_maxrec(best_rec, pack);
    for(auto rec : new_rec)
    {
        car.list_of_free_rec.push_back(rec);
    }

    // Remove overlap part
    remove_overlap(car, pack);

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
void Solve_best_fit()
{
    sort(item + 1, item + N_items + 1, compare_item_by_longer_side);
    sort(bin + 1, bin + N_bins + 1, compare_bin_by_density);
    for(int i = 1; i <= N_items; ++i)
    {
        // Bin First Fit: choose bin that best fit
        int best_bin        = 0;
        pair <int, int> best_score;
        best_score.first    = inf;
        best_score.second   = inf;
        for(int j = 1; j <= N_bins; ++j)
        {
            pair <pair <Free_Rectangles, int>, pair <bool, bool>> best = best_ranking(bin[j], item[i]);
            bool check_exist            = best.second.second;
            if(!check_exist) continue;
            Free_Rectangles best_rec    = best.first.first;
            bool rotated                = best.second.first;
            if(compare_ranking_rec_BSS(score_rec(best_rec, item[i], rotated), best_score))
            {
                best_score  = score_rec(best_rec, item[i], rotated);
                best_bin    = j;
            }
        }
        if(best_bin > 0) insert_item_maxrec(bin[best_bin], item[i]);
            else cout << "Nope: " << item[i].id << endl;
    }
}

void Solve_first_fit()
{
    sort(item + 1, item + N_items + 1, compare_item_by_longer_side);
    sort(bin + 1, bin + N_bins + 1, compare_bin_by_density);
    for(int i = 1; i <= N_items; ++i)
    {
        // Bin First Fit: choose bin that first fit
        for(int j = 1; j <= N_bins; ++j)
            if(insert_item_maxrec(bin[j],item[i])) break;
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
    Solve_first_fit();
    Print();

    // Print additional informations
    cout << "Status: "                  << "None"       << endl;
    cout << "Time limit: "              << "None"       << endl;
    cout << "Running time: ";
    cout.precision(20);
    cout << float(clock() - start_timing)/CLOCKS_PER_SEC << endl;
}
