/*----------------- MAXIMAL RECTANGLES ALGORITHM -----------------*/

/*----------------- PACKING ITEMS -----------------*/
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
bool check_intersec_maxrec(Free_Rectangles rec, Items pack)
{
    if(pack.corner_x >= rec.corner_x + rec.width)   return 0;
    if(pack.corner_y >= rec.corner_y + rec.height)  return 0;
    if(pack.corner_x + pack.width <= rec.corner_x)  return 0;
    if(pack.corner_y + pack.height <= rec.corner_y) return 0;
    return 1;
}

// Find overlap part
Free_Rectangles find_overlap_maxrec(Free_Rectangles rec, Items pack)
{
    Free_Rectangles overlap_rec;
    overlap_rec.corner_x    = max(rec.corner_x, pack.corner_x);
    overlap_rec.corner_y    = max(rec.corner_y, pack.corner_y);
    overlap_rec.width       = min(rec.corner_x + rec.width, pack.corner_x + pack.width) - overlap_rec.corner_x;
    overlap_rec.height      = min(rec.corner_y + rec.height, pack.corner_y + pack.height) - overlap_rec.corner_y;
    return overlap_rec;
}

// Split the intersected rec into smaller free_recs
vector <Free_Rectangles> split_intersect_maxrec (Free_Rectangles initial_rec, Free_Rectangles overlap_rec)
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
bool check_covered_maxrec(Free_Rectangles rec_covering, Free_Rectangles rec_covered)
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
void remove_covered_rec_maxrec(Bins &car)
{
    for(unsigned int i = 0; i < car.list_of_free_rec.size(); ++i)
    {
        Free_Rectangles first = car.list_of_free_rec[i];
        for(unsigned int j = i + 1; j < car.list_of_free_rec.size(); ++j)
        {
            Free_Rectangles second = car.list_of_free_rec[j];
            // If rec i cover rec j then delete rec j
            if(check_covered_maxrec(first, second))
            {
                car.list_of_free_rec.erase(car.list_of_free_rec.begin() + j);
                --j;
                continue;
            }
            // If rec j cover rec i then delete rec i
            if(check_covered_maxrec(second, first))
            {
                car.list_of_free_rec.erase(car.list_of_free_rec.begin() + i);
                --i;
                break;
            }
        }
    }
}

// Remove overlap part
void remove_overlap_maxrec(Bins &car, Items pack)
{
    for(unsigned int i = 0; i < car.list_of_free_rec.size(); ++i)
    {
        Free_Rectangles rec = car.list_of_free_rec[i];
        if(check_intersec_maxrec(rec, pack))
        {
            Free_Rectangles overlap_rec = find_overlap_maxrec(rec, pack);
            vector <Free_Rectangles> new_rec = split_intersect_maxrec(rec, overlap_rec);
            car.list_of_free_rec.erase(car.list_of_free_rec.begin() + i);
            for(auto free_rec : new_rec)
            {
                car.list_of_free_rec.push_back(free_rec);
            }
            --i;
        }
    }
    remove_covered_rec_maxrec(car);
}

// Check if item find a possible free_rec in the bin for inserting process
bool insert_item_maxrec(Bins &car, Items &pack)
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
    vector <Free_Rectangles> new_rec = spliting_process_maxrec(best_rec, pack);
    for(auto rec : new_rec)
    {
        car.list_of_free_rec.push_back(rec);
    }

    // Remove overlap part
    remove_overlap_maxrec(car, pack);

    return 1;
}


/*----------------- BIN CHOOSING -----------------*/
void Solve_maxrec()
{
    for(int i = 1; i <= N_items; ++i)
    {
        // Bin First Fit: choose bin that first fit
        for(int j = 1; j <= N_bins; ++j)
            if(insert_item_maxrec(bin[j],item[i])) break;
    }
}
