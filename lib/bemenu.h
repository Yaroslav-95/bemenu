#ifndef _BEMENU_H_
#define _BEMENU_H_

/**
 * @file bemenu.h
 *
 * Public API header.
 */

struct bm_renderer;
struct bm_menu;
struct bm_item;

#include <stdbool.h>
#include <stdint.h>

/**
 * @defgroup Library
 * @brief Library functions.
 *
 * Query library version, etc...
 */

/**
 * @defgroup Menu
 * @brief Menu container.
 *
 * Holds all the items, runs logic and gets rendered.
 */

/**
 * @defgroup Item
 * @brief Item container.
 *
 * Contains properties for visual representation of item.
 */

/**
 * @addtogroup Library
 * @{ */

/**
 * @name Library Initialization
 * @{ */

/**
 * Init bemenu, loads up the renderers.
 *
 * You can force single renderer with BEMENU_RENDERER env variable,
 * and directory containing renderers with BEMENU_RENDERERS env variable.
 *
 * @return true on success, false on failure.
 */
bool bm_init(void);

/**
 * Get list of available renderers.
 *
 * @param out_nmemb Reference to uint32_t where total count of returned renderers will be stored.
 * @return Pointer to array of bm_renderer instances.
 */
const struct bm_renderer** bm_get_renderers(uint32_t *out_nmemb);

/** @} Library Initialization */

/**
 * @name Library Version
 * @{ */

/**
 * Get version of the library in 'major.minor.patch' format.
 *
 * @see @link http://semver.org/ Semantic Versioning @endlink
 *
 * @return Null terminated C "string" to version string.
 */
const char* bm_version(void);

/**  @} Library Version */

/**  @} Library */

/**
 * @addtogroup Renderer
 * @{ */

/**
 * Prioritories for renderer plugins.
 */
enum bm_priorty {
    /**
     * Renderer runs in terminal.
     */
    BM_PRIO_TERMINAL,

    /**
     * Renderer runs in GUI.
     */
    BM_PRIO_GUI,
};

/**
 * Get name of the renderer.
 *
 * @param renderer bm_renderer instance.
 * @return Null terminated C "string" to renderer's name.
 */
const char* bm_renderer_get_name(const struct bm_renderer *renderer);

/**
 * Get priorty of the renderer.
 *
 * @param renderer bm_renderer instance.
 * @return bm_priorty enum value.
 */
enum bm_priorty bm_renderer_get_priorty(const struct bm_renderer *renderer);

/**
 * @} Renderer */

/**
 * @addtogroup Menu
 * @{ */

/**
 * Filter mode constants for bm_menu instance filter mode.
 *
 * @link ::bm_filter_mode BM_FILTER_MODE_LAST @endlink is provided for enumerating filter modes.
 * Using it as filter mode however provides exactly same functionality as BM_FILTER_MODE_DMENU.
 */
enum bm_filter_mode {
    BM_FILTER_MODE_DMENU,
    BM_FILTER_MODE_DMENU_CASE_INSENSITIVE,
    BM_FILTER_MODE_LAST
};

/**
 * Result constants from bm_menu_run_with_key function.
 *
 * - @link ::bm_run_result BM_RUN_RESULT_RUNNING @endlink means that menu is running and thus should be still renderer && ran.
 * - @link ::bm_run_result BM_RUN_RESULT_SELECTED @endlink means that menu was closed and items were selected.
 * - @link ::bm_run_result BM_RUN_RESULT_CANCEL @endlink means that menu was closed and selection was canceled.
 */
enum bm_run_result {
    BM_RUN_RESULT_RUNNING,
    BM_RUN_RESULT_SELECTED,
    BM_RUN_RESULT_CANCEL,
};

/**
 * Key constants.
 *
 * @link ::bm_key BM_KEY_LAST @endlink is provided for enumerating keys.
 */
enum bm_key {
    BM_KEY_NONE,
    BM_KEY_UP,
    BM_KEY_DOWN,
    BM_KEY_LEFT,
    BM_KEY_RIGHT,
    BM_KEY_HOME,
    BM_KEY_END,
    BM_KEY_PAGE_UP,
    BM_KEY_PAGE_DOWN,
    BM_KEY_SHIFT_PAGE_UP,
    BM_KEY_SHIFT_PAGE_DOWN,
    BM_KEY_BACKSPACE,
    BM_KEY_DELETE,
    BM_KEY_LINE_DELETE_LEFT,
    BM_KEY_LINE_DELETE_RIGHT,
    BM_KEY_WORD_DELETE,
    BM_KEY_TAB,
    BM_KEY_SHIFT_TAB,
    BM_KEY_ESCAPE,
    BM_KEY_RETURN,
    BM_KEY_SHIFT_RETURN,
    BM_KEY_CONTROL_RETURN,
    BM_KEY_UNICODE,
    BM_KEY_LAST
};

/**
 * Colorable element constants.
 *
 * @link ::bm_color BM_COLOR_LAST @endlink is provided for enumerating colors.
 */
enum bm_color {
    BM_COLOR_BG,
    BM_COLOR_TITLE_BG,
    BM_COLOR_TITLE_FG,
    BM_COLOR_FILTER_BG,
    BM_COLOR_FILTER_FG,
    BM_COLOR_ITEM_BG,
    BM_COLOR_ITEM_FG,
    BM_COLOR_HIGHLIGHTED_BG,
    BM_COLOR_HIGHLIGHTED_FG,
    BM_COLOR_SELECTED_BG,
    BM_COLOR_SELECTED_FG,
    BM_COLOR_LAST
};

/**
 * @name Menu Memory
 * @{ */

/**
 * Create new bm_menu instance.
 *
 * If **NULL** is used as renderer, auto-detection will be used or the renderer with the name pointed by BEMENU_BACKEND env variable.
 * It's good idea to use NULL, if you want user to have control over the renderer with this env variable.
 *
 * @param renderer Name of renderer to be used for this instance, pass **NULL** for auto-detection.
 * @return bm_menu for new menu instance, **NULL** if creation failed.
 */
struct bm_menu* bm_menu_new(const char *renderer);

/**
 * Release bm_menu instance.
 *
 * @param menu bm_menu instance to be freed from memory.
 */
void bm_menu_free(struct bm_menu *menu);

/**
 * Release items inside bm_menu instance.
 *
 * @param menu bm_menu instance which items will be freed from memory.
 */
void bm_menu_free_items(struct bm_menu *menu);

/**  @} Menu Memory */

/**
 * @name Menu Properties
 * @{ */

/**
 * Set userdata pointer to bm_menu instance.
 * Userdata will be carried unmodified by the instance.
 *
 * @param menu bm_menu instance where to set userdata pointer.
 * @param userdata Pointer to userdata.
 */
void bm_menu_set_userdata(struct bm_menu *menu, void *userdata);

/**
 * Get userdata pointer from bm_menu instance.
 *
 * @param menu bm_menu instance which userdata pointer to get.
 * @return Pointer for unmodified userdata.
 */
void* bm_menu_get_userdata(struct bm_menu *menu);

/**
 * Set highlight prefix.
 * This is shown on vertical list mode only.
 *
 * @param menu bm_menu instance where to set highlight prefix.
 * @param prefix Null terminated C "string" to act as prefix for highlighted item. May be set **NULL** for none.
 */
void bm_menu_set_prefix(struct bm_menu *menu, const char *prefix);

/**
 * Get highlight prefix.
 *
 * @param menu bm_menu instance where to get highlight prefix.
 * @param Const pointer to current highlight prefix, may be **NULL** if empty.
 */
const char* bm_menu_get_prefix(struct bm_menu *menu);

/**
 * Set filter text to bm_menu instance.
 *
 * @param menu bm_menu instance where to set filter.
 * @param filter Null terminated C "string" to act as filter. May be set **NULL** for none.
 */
void bm_menu_set_filter(struct bm_menu *menu, const char *filter);

/**
 * Get filter text from bm_menu instance.
 *
 * @param menu bm_menu instance where to get filter.
 * @return Const pointer to current filter text, may be **NULL** if empty.
 */
const char* bm_menu_get_filter(struct bm_menu *menu);

/**
 * Set active filter mode to bm_menu instance.
 *
 * @param menu bm_menu instance where to set filter mode.
 * @param mode bm_filter_mode constant.
 */
void bm_menu_set_filter_mode(struct bm_menu *menu, enum bm_filter_mode mode);

/**
 * Get active filter mode from bm_menu instance.
 *
 * @param menu bm_menu instance where to get filter mode.
 * @return bm_filter_mode constant.
 */
enum bm_filter_mode bm_menu_get_filter_mode(const struct bm_menu *menu);

/**
 * Set amount of max vertical lines to be shown.
 * Some renderers such as ncurses may ignore this when it does not make sense.
 *
 * @param menu bm_menu instance where to set max vertical line amount.
 * @param lines 0 for single line layout, > 0 to show that many lines.
 */
void bm_menu_set_lines(struct bm_menu *menu, uint32_t lines);

/**
 * Get amount of max vertical lines to be shown.
 *
 * @param menu bm_menu instance where to get max vertical line amount.
 * @return uint32_t for max amount of vertical lines to be shown.
 */
uint32_t bm_menu_get_lines(struct bm_menu *menu);

/**
 * Set selection wrapping on/off.
 *
 * @param menu bm_menu instance where to toggle selection wrapping.
 * @param wrap true/false.
 */
void bm_menu_set_wrap(struct bm_menu *menu, bool wrap);

/**
 * Get selection wrapping state.
 *
 * @param menu bm_menu instance where to get selection wrapping state.
 * @return int for wrap state.
 */
bool bm_menu_get_wrap(const struct bm_menu *menu);

/**
 * Set title to bm_menu instance.
 *
 * @param menu bm_menu instance where to set title.
 * @param title C "string" to set as title, can be **NULL** for empty title.
 * @return true if set was succesful, false if out of memory.
 */
bool bm_menu_set_title(struct bm_menu *menu, const char *title);

/**
 * Get title from bm_menu instance.
 *
 * @param menu bm_menu instance where to get title from.
 * @return Pointer to null terminated C "string", can be **NULL** for empty title.
 */
const char* bm_menu_get_title(const struct bm_menu *menu);

/**
 * Set font description to bm_menu instance.
 *
 * @param menu bm_menu instance where to set font.
 * @param font C "string" for a **pango style font description**, can be **NULL** for default (Terminus 9).
 * @return true if set was succesful, false if out of memory.
 */
bool bm_menu_set_font(struct bm_menu *menu, const char *font);

/**
 * Get font description from bm_menu instance.
 *
 * @param menu bm_menu instance where to get font description from.
 * @return Pointer to null terminated C "string".
 */
const char* bm_menu_get_font(const struct bm_menu *menu);

/**
 * Set a hexadecimal color for element.
 *
 * @param menu bm_menu instance where to set color.
 * @param color bm_color type.
 * @param hex Color in hexadecimal format starting with '#'.
 * @return true if set was succesful, false if out of memory.
 */
bool bm_menu_set_color(struct bm_menu *menu, enum bm_color color, const char *hex);

/**
 * Get hexadecimal color for element.
 *
 * @param menu bm_menu instance where to get color from.
 * @param color bm_color type.
 * @return Pointer to null terminated C "string".
 */
const char* bm_menu_get_color(const struct bm_menu *menu, enum bm_color color);

/**
 * Display menu at bottom of the screen.
 * This may be no-op on some renderers (curses, wayland)
 *
 * @param menu bm_menu instance to set bottom mode for.
 * @param bottom true for bottom mode, false for top mode.
 */
void bm_menu_set_bottom(struct bm_menu *menu, bool bottom);

/**
 * Is menu being displayed at bottom of the screen?
 *
 * @param menu bm_menu instance where to get bottom mode from.
 * @return true if bottom mode, false otherwise.
 */
bool bm_menu_get_bottom(struct bm_menu *menu);

/**
 * Display menu at monitor index.
 * Indices start from 1, pass 0 for active monitor (default).
 * If index is more than amount of monitors, the monitor with highest index will be selected.
 *
 * @param menu bm_menu instance to set monitor for.
 * @param monitor Monitor index starting from 1.
 */
void bm_menu_set_monitor(struct bm_menu *menu, uint32_t monitor);

/**
 * Return index for current monitor.
 *
 * @param menu bm_menu instance where to get current monitor from.
 * @return Monitor index starting from 1.
 */
uint32_t bm_menu_get_monitor(struct bm_menu *menu);

/**
 * Tell renderer to grab keyboard.
 * This only works with x11 renderer.
 *
 * @param menu bm_menu instance to set grab for.
 * @param grab true for grab, false for ungrab.
 */
void bm_menu_grab_keyboard(struct bm_menu *menu, bool grab);

/**
 * Is keyboard grabbed for bm_menu?
 *
 * @param menu bm_menu instance where to check grab status from.
 * @return true if grabbed, false if not.
 */
bool bm_menu_is_keyboard_grabbed(struct bm_menu *menu);

/**  @} Properties */

/**
 * @name Menu Items
 * @{ */

/**
 * Add item to bm_menu instance at specific index.
 *
 * @param menu bm_menu instance where item will be added.
 * @param item bm_item instance to add.
 * @param index Index where item will be added.
 * @return true on successful add, false on failure.
 */
bool bm_menu_add_item_at(struct bm_menu *menu, struct bm_item *item, uint32_t index);

/**
 * Add item to bm_menu instance.
 *
 * @param menu bm_menu instance where item will be added.
 * @param item bm_item instance to add.
 * @return true on successful add, false on failure.
 */
bool bm_menu_add_item(struct bm_menu *menu, struct bm_item *item);

/**
 * Remove item from bm_menu instance at specific index.
 *
 * @warning The item won't be freed, use bm_item_free to do that.
 *
 * @param menu bm_menu instance from where item will be removed.
 * @param index Index of item to remove.
 * @return true on successful add, false on failure.
 */
bool bm_menu_remove_item_at(struct bm_menu *menu, uint32_t index);

/**
 * Remove item from bm_menu instance.
 *
 * @warning The item won't be freed, use bm_item_free to do that.
 *
 * @param menu bm_menu instance from where item will be removed.
 * @param item bm_item instance to remove.
 * @return true on successful add, false on failure.
 */
bool bm_menu_remove_item(struct bm_menu *menu, struct bm_item *item);

/**
 * Highlight item in menu by index.
 *
 * @param menu bm_menu instance from where to highlight item.
 * @param index Index of item to highlight.
 * @return true on successful highlight, false on failure.
 */
bool bm_menu_set_highlighted_index(struct bm_menu *menu, uint32_t index);

/**
 * Highlight item in menu.
 *
 * @param menu bm_menu instance from where to highlight item.
 * @param item bm_item instance to highlight.
 * @return true on successful highlight, false on failure.
 */
bool bm_menu_set_highlighted_item(struct bm_menu *menu, struct bm_item *item);

/**
 * Get highlighted item from bm_menu instance.
 *
 * @warning The pointer returned by this function may be invalid after items change.
 *
 * @param menu bm_menu instance from where to get highlighted item.
 * @return Selected bm_item instance, **NULL** if none highlighted.
 */
struct bm_item* bm_menu_get_highlighted_item(const struct bm_menu *menu);

/**
 * Set selected items to bm_menu instance.
 *
 * @param menu bm_menu instance where items will be set.
 * @param items Array of bm_item pointers to set.
 * @param nmemb Total count of items in array.
 * @return true on successful set, false on failure.
 */
bool bm_menu_set_selected_items(struct bm_menu *menu, struct bm_item **items, uint32_t nmemb);

/**
 * Get selected items from bm_menu instance.
 *
 * @warning The pointer returned by this function may be invalid after selection or items change.
 *
 * @param menu bm_menu instance from where to get selected items.
 * @param out_nmemb Reference to uint32_t where total count of returned items will be stored.
 * @return Pointer to array of bm_item pointers.
 */
struct bm_item** bm_menu_get_selected_items(const struct bm_menu *menu, uint32_t *out_nmemb);

/**
 * Set items to bm_menu instance.
 * Will replace all the old items on bm_menu instance.
 *
 * If items is **NULL**, or nmemb is zero, all items will be freed from the menu.
 *
 * @param menu bm_menu instance where items will be set.
 * @param items Array of bm_item pointers to set.
 * @param nmemb Total count of items in array.
 * @return true on successful set, false on failure.
 */
bool bm_menu_set_items(struct bm_menu *menu, const struct bm_item **items, uint32_t nmemb);

/**
 * Get items from bm_menu instance.
 *
 * @warning The pointer returned by this function may be invalid after removing or adding new items.
 *
 * @param menu bm_menu instance from where to get items.
 * @param out_nmemb Reference to uint32_t where total count of returned items will be stored.
 * @return Pointer to array of bm_item pointers.
 */
struct bm_item** bm_menu_get_items(const struct bm_menu *menu, uint32_t *out_nmemb);

/**
 * Get filtered (displayed) items from bm_menu instance.
 *
 * @warning The pointer returned by this function _will_ be invalid when menu internally filters its list again.
 *          Do not store this pointer.
 *
 * @param menu bm_menu instance from where to get filtered items.
 * @param out_nmemb Reference to uint32_t where total count of returned items will be stored.
 * @return Pointer to array of bm_item pointers.
 */
struct bm_item** bm_menu_get_filtered_items(const struct bm_menu *menu, uint32_t *out_nmemb);

/**  @} Menu Items */

/**
 * @name Menu Logic
 * @{ */

/**
 * Render bm_menu instance using chosen renderer.
 *
 * This function may block on **wayland** and **x11** renderer.
 *
 * @param menu bm_menu instance to be rendered.
 */
void bm_menu_render(const struct bm_menu *menu);

/**
 * Trigger filtering of menu manually.
 * This is useful when adding new items and want to dynamically see them filtered.
 *
 * Do note that filtering might be heavy, so you should only call it after batch manipulation of items.
 * Not after manipulation of each single item.
 *
 * @param menu bm_menu instance which to filter.
 */
void bm_menu_filter(struct bm_menu *menu);

/**
 * Poll key and unicode from underlying UI toolkit.
 *
 * This function will block on **curses** renderer.
 *
 * @param menu bm_menu instance from which to poll.
 * @param out_unicode Reference to uint32_t.
 * @return bm_key for polled key.
 */
enum bm_key bm_menu_poll_key(struct bm_menu *menu, uint32_t *out_unicode);

/**
 * Advances menu logic with key and unicode as input.
 *
 * @param menu bm_menu instance to be advanced.
 * @param key Key input that will advance menu logic.
 * @param unicode Unicode input that will advance menu logic.
 * @return bm_run_result for menu state.
 */
enum bm_run_result bm_menu_run_with_key(struct bm_menu *menu, enum bm_key key, uint32_t unicode);

/**  @} Menu Logic */

/**  @} Menu */

/**
 * @addtogroup Item
 * @{ */

/**
 * @name Item Memory
 * @{ */

/**
 * Allocate a new item.
 *
 * @param text Pointer to null terminated C "string", can be **NULL** for empty text.
 * @return bm_item for new item instance, **NULL** if creation failed.
 */
struct bm_item* bm_item_new(const char *text);

/**
 * Release bm_item instance.
 *
 * @param item bm_item instance to be freed from memory.
 */
void bm_item_free(struct bm_item *item);

/**  @} Item Memory */

/**
 * @name Item Properties
 * @{ */

/**
 * Set userdata pointer to bm_item instance.
 * Userdata will be carried unmodified by the instance.
 *
 * @param item bm_item instance where to set userdata pointer.
 * @param userdata Pointer to userdata.
 */
void bm_item_set_userdata(struct bm_item *item, void *userdata);

/**
 * Get userdata pointer from bm_item instance.
 *
 * @param item bm_item instance which userdata pointer to get.
 * @return Pointer for unmodified userdata.
 */
void* bm_item_get_userdata(struct bm_item *item);

/**
 * Set text to bm_item instance.
 *
 * @param item bm_item instance where to set text.
 * @param text C "string" to set as text, can be **NULL** for empty text.
 * @return true if set was succesful, false if out of memory.
 */
bool bm_item_set_text(struct bm_item *item, const char *text);

/**
 * Get text from bm_item instance.
 *
 * @param item bm_item instance where to get text from.
 * @return Pointer to null terminated C "string", can be **NULL** for empty text.
 */
const char* bm_item_get_text(const struct bm_item *item);

/**  @} Item Properties */

/**  @} Item */

#endif /* _BEMENU_H_ */

/* vim: set ts=8 sw=4 tw=0 :*/
