/*
 * imap/list.c - issues and handles IMAP LIST commands
 */
#include "internal/imap.h"
#include "util/list.h"
#include "util/stringop.h"

void imap_list(struct imap_connection *imap, imap_callback_t callback,
		void *data, const char *refname, const char *boxname) {
	imap_send(imap, callback, "LIST \"%s\" \"%s\"", refname, boxname);
}

void handle_imap_list(struct imap_connection *imap, const char *token,
		const char *cmd, imap_arg_t *args) {
	// TODO
}
