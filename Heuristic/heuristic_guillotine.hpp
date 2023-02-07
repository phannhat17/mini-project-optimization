/*----------------- GUILLOTINE ALGORITHM -----------------*/

/*----------------- PACKING ITEMS -----------------*/
// Split initial free_rec after having an insertion
vector <Free_Rectangles> spliting_process_guillotine(bool horizontal, Free_Rectangles rec, Items pack)
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
vector <Free_Rectangles> spliting_guillotine(Free_Rectangles rec, Items pack)
{
    // Split by Shorter Axis: split on the horizontal axis if rec.width <= rec.height
    return spliting_process_guillotine(rec.width <= rec.height, rec, pack);
}

// Merge two initial free_recs into a larger free_rec if possible
void merge_rec_guillotine(Bins &car)
{
    vector <Free_Rectangles> match_width, match_height;

    for(unsigned int i = 0; i < car.list_of_free_rec.size(); ++i)
    {
        Free_Rectangles first   = car.list_of_free_rec[i];
        bool check_exist_width  = 0;
        bool check_exist_height = 0;
        unsigned int pos_check_width     = 0;
        unsigned int pos_check_height    = 0;
        // Loop to find a mergable free_rec for the rec i
        for(unsigned int j = 0; j < car.list_of_free_rec.size(); ++j)
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

// Check if item find a possible free_rec in the bin for inserting process
bool insert_item_guillotine(Bins &car, Items &pack)
{
    pair <pair <Free_Rectangles, unsigned int>, pair <bool, bool>> best_ranking_return;
    best_ranking_return         = best_ranking(car, pack);

    // If the free_rec which fits the item does not exist
    if(!best_ranking_return.second.second) return 0;

    // If the free_rec exists
    pack.pos_bin                = car.id;
    Free_Rectangles best_rec    = best_ranking_return.first.first;
    unsigned int best_pos       = best_ranking_return.first.second;
    int rotated                 = best_ranking_return.second.first;
    // Add the item into the choosen free_rec
    add_item(car, pack, rotated, best_rec.corner_x, best_rec.corner_y);
    // Replace the used free_rec by the new splited rec(s)
    car.list_of_free_rec.erase(car.list_of_free_rec.begin() + best_pos);
    vector <Free_Rectangles> new_rec = spliting_guillotine(best_rec, pack);
    for(auto rec : new_rec)
    {
        car.list_of_free_rec.push_back(rec);
    }
    // Merge the free_rec if possible
    merge_rec_guillotine(car);

    return 1;
}


/*----------------- BIN CHOOSING -----------------*/
void Solve_guillotine()
{
    for(int i = 1; i <= N_items; ++i)
    {
        // Bin First Fit: choose bin that first fit
        for(int j = 1; j <= N_bins; ++j)
            if(insert_item_guillotine(bin[j],item[i])) break;
    }
}
