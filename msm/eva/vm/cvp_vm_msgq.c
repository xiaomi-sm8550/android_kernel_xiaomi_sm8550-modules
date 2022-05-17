/* SPDX-License-Identifier: GPL-2.0-only
 *
 * Copyright (c) 2022, Qualcomm Innovation Center, Inc. All rights reserved.
 */

#include <linux/kthread.h>
#include "cvp_vm_msgq.h"
#include "msm_cvp_debug.h"

/**
 * cvp_msgq_receiver - thread function that receive msg from gunyah msgq
 * data: cvp_msgq_drv pointer
 *
 * Note: single thread. If the sub-function or global data used in this
 *       function is also used somehwere else, please add rx_lock.
 */
static int cvp_msgq_receiver(void *data)
{
	struct cvp_msgq_drv *msgq_drv = data;

	if (IS_ERR_OR_NULL(msgq_drv))
		return -EINVAL;

	return 0;
}

static int cvp_complete_msgq_init(struct cvp_msgq_drv *msgq_drv)
{
	int i;

	msgq_drv->receiver_thread = kthread_run(
			cvp_msgq_receiver,
			(void *)msgq_drv,
			"CVP msgq receiver");
	if (IS_ERR_OR_NULL(msgq_drv->receiver_thread)) {
		dprintk(CVP_ERR, "Failed to start msgq receiver thread\n");
		return -EINVAL;
	}

	mutex_init(&msgq_drv->ipc_lock);

	for (i = 0; i <= CVP_MAX_IPC_CMD; i++)
		init_completion(&msgq_drv->completions[i]);

	return 0;
}

static int cvp_msgq_cb(struct notifier_block *nb,
		unsigned long cmd, void *data)
{
	struct gh_rm_notif_vm_status_payload *vm_status_payload;
	struct cvp_gh_msgq_config *msgq_config;
	struct cvp_msgq_drv *msgq_drv;
	gh_vmid_t peer_vmid;
	gh_vmid_t self_vmid;
	int rc;


	if (IS_ERR_OR_NULL(nb))
		return -EINVAL;

	msgq_config = container_of(nb, struct cvp_gh_msgq_config, rm_nb);
	msgq_drv = container_of(msgq_config, struct cvp_msgq_drv, config);

	if (cmd != GH_RM_NOTIF_VM_STATUS)
		return NOTIFY_DONE;

	/**
	 * Check VM status, only GH_TRUSTED_VM notification activate
	 * Gunyah msgq registration
	 */
	vm_status_payload = (struct gh_rm_notif_vm_status_payload *)data;

	if (vm_status_payload->vm_status != GH_RM_VM_STATUS_READY)
		return -12;

	if (gh_rm_get_vmid(msgq_config->peer_id, &peer_vmid))
		return -13;

	if (gh_rm_get_vmid(GH_PRIMARY_VM, &self_vmid))
		return -14;

	if (peer_vmid != vm_status_payload->vmid)
		return NOTIFY_DONE;

	dprintk(CVP_VM, "%s vmid=%d, peer_vmid=%d\n", vm_status_payload->vmid,
			peer_vmid);

	if (msgq_config->handle)
		return -15;

	msgq_config->handle = gh_msgq_register(GH_MSGQ_LABEL_EVA);
	if (IS_ERR(msgq_config->handle)) {
		rc = PTR_ERR(msgq_drv->config.handle);
		dprintk(CVP_ERR, "PVM failed to register msgq %d\n", rc);
		return rc;
	}

	rc = cvp_complete_msgq_init(msgq_drv);

	return rc;
}

static int cvp_msgq_init(struct cvp_msgq_drv *msgq_drv)
{
	int rc = 0;

	msgq_drv->config.label = GH_MSGQ_LABEL_EVA;
	msgq_drv->config.handle = NULL;
#ifndef CONFIG_EVA_TVM
	/* PVM init */
	msgq_drv->config.peer_id = GH_TRUSTED_VM;
	msgq_drv->config.rm_nb.notifier_call = cvp_msgq_cb;
	rc = gh_rm_register_notifier(&msgq_drv->config.rm_nb);
	if (rc) {
		dprintk(CVP_ERR, "PVM Fail register msgq notifier %d\n", rc);
		return rc;
	}
#else
	/* TVM init */
	msgq_drv->config.handle = gh_msgq_register(GH_MSGQ_LABEL_EVA);
	if (IS_ERR(msgq_drv->config.handle)) {
		rc = PTR_ERR(msgq_drv->config.handle);
		dprintk(CVP_ERR, "TVM failed to register msgq %d\n", rc);
		return rc;
	}
	rc = cvp_msgq_complete_init(msgq_drv);
#endif
	return rc;
}

static int cvp_msgq_deinit(struct cvp_msgq_drv *msgq_drv)
{
	if (msgq_drv->receiver_thread)
		kthread_stop(msgq_drv->receiver_thread);

	return 0;
}

static int cvp_msgq_send(struct cvp_msgq_drv *msgq_drv,
		void *msg, size_t msg_size)
{
	return 0;
}

static int cvp_msgq_receive(struct cvp_msgq_drv *msgq_drv)
{
	return 0;
}

static struct cvp_msgq_ops msgq_ops = {
	.msgq_init = cvp_msgq_init,
	.msgq_deinit = cvp_msgq_deinit,
	.msgq_send = cvp_msgq_send,
	.msgq_receive = cvp_msgq_receive,
};

struct cvp_msgq_drv cvp_ipc_msgq = {
	.ops = &msgq_ops,
};
