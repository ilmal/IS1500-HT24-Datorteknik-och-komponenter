void interact_with_tile(struct player player, struct tile current_tile, struct tile map[10][10], int switch_values);
void display_frame(struct player player, struct tile current_tile, struct tile map[10][10]);
int string_compare(const char *str1, const char *str2);

void int_to_str(int num, char *str);
void append_str(char *dest, const char *src);

extern void print(const char *);
