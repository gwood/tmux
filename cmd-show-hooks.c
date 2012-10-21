/* $Id$ */

/*
 * Copyright (c) 2012 Thomas Adam <thomas@xteddy.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/types.h>

#include <ctype.h>
#include <stdlib.h>

#include <string.h>

#include "tmux.h"

enum cmd_retval cmd_show_hooks_exec(struct cmd *, struct cmd_ctx *);

const struct cmd_entry cmd_show_hooks_entry = {
	"show-hooks", NULL,
	"gt:", 0, 1,
	"[-g] " CMD_TARGET_SESSION_USAGE,
	0,
	NULL,
	NULL,
	cmd_show_hooks_exec
};

enum cmd_retval
cmd_show_hooks_exec(struct cmd *self, struct cmd_ctx *ctx)
{
	struct args	*args = self->args;
	struct session	*s;
	struct hook	*hook;
	struct hooks	*hooks_ent;
	struct cmd	*cmd;
	char		 tmp[BUFSIZ];
	size_t		 used;

	if ((s = cmd_find_session(ctx, args_get(args, 't'), 0)) == NULL)
		return (CMD_RETURN_ERROR);

	hooks_ent = args_has(args, 'g') ? &global_hooks : &s->hooks;

	RB_FOREACH(hook, hooks, hooks_ent) {
		used = xsnprintf(tmp, sizeof tmp, "%s -> ", hook->name);
		cmd_list_print(hook->cmdlist, tmp + used, (sizeof tmp) - used);
		ctx->print(ctx, "%s", tmp);
	}
	return (CMD_RETURN_NORMAL);
}