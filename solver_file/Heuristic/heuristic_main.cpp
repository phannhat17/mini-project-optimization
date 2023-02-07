/*----------------- MAIN PROGRAM ------------------*/

#include "declaration.hpp"

#include "heuristic_guillotine.hpp"
#include "heuristic_maxrec.hpp"

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

    sort(item + 1, item + N_items + 1, compare_item_by_longer_side);
    sort(bin + 1, bin + N_bins + 1, compare_bin_by_density);
}


/*----------------- RESET -----------------*/
void reset()
{
    total_cost = 0;

    // Save packing solution for guillotine algorithm and reset items for next algorithm
    for(int i = 1; i <= N_items; ++i)
    {
        // Save guillotine solution
        item_guillotine[i] = item[i];
        // Reset items
        item[i].corner_x = 0;
        item[i].corner_y = 0;
    }

    // Reset bins for next algorithm
    for(int j = 1; j <= N_bins; ++j)
    {
        // Reset bins
        bin[j].free_area = bin[j].area;
        bin[j].list_of_items.clear();
        bin[j].list_of_free_rec.clear();
        Free_Rectangles first_rec;
        first_rec.width     = bin[j].width;
        first_rec.height    = bin[j].height;
        first_rec.corner_x  = first_rec.corner_y = 0;
        first_rec.area      = first_rec.width * first_rec.height;
        bin[j].list_of_free_rec.push_back(first_rec);
    }
}


/*----------------- SOLVE -----------------*/
void Solve()
{
    Enter();
    // Run Gullotine algorithm
    Solve_guillotine();
    pair <int, int> guillotine_result   = calculate_solution();

    reset();

    // Run Maxrec algorithm
    Solve_maxrec();
    pair <int, int> maxrec_result       = calculate_solution();

    if(guillotine_result.first < maxrec_result.first)
    {
        total_cost  = guillotine_result.first;
        bin_used    = guillotine_result.second;
    }
    else
    {
        total_cost  = maxrec_result.first;
        bin_used    = maxrec_result.second;
        check_algorithm = 1;
    }
}


/*----------------- PRINT OUTPUT -----------------*/
void Print()
{
    // Print the input sizes
    cout << "Number of item given: "    << N_items      << '\n';
    cout << "Number of bin given: "     << N_bins       << '\n';

    // Uncomment for more detailed information of packed items in each used bin
//    cout << "Algorithm used: ";
//    if(!check_algorithm)    cout << "Guillotine"            << '\n';
//    else                    cout << "Maximal Rectangles"    << '\n';
    checking_status(check_algorithm);

    // Print the solution
    cout << "Number of bin used: "      << bin_used     << '\n';
    cout << "Total cost: "              << total_cost   << '\n';
}


/*----------------- MAIN -----------------*/
signed main(int argv, char* argc[])
{
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    if(argv < 2)
    {
        cout << "Error: No file name provided." << '\n';
        return 0;
    }
    freopen(argc[1], "r", stdin);
//    freopen("testing.data", "r", stdin);
//    freopen("testing.out", "w", stdout);

    // Start timing process
    clock_t start_timing    = clock();

    // Run solver
    Solve();

    // Stop timing process
    clock_t end_timing      = clock();

    // Print output
    Print();
    // Print additional informations
    cout << "Status: "                  << "None"       << '\n';
    cout << "Time limit: "              << "None"       << '\n';
    cout << "Running time: ";
    cout.precision(20);
    cout << float(end_timing - start_timing)/CLOCKS_PER_SEC << '\n';
}
