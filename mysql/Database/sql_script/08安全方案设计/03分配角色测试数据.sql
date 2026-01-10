-- @author 巷北  
-- @time 2026-01-01 23:17:37  

-- 给前台员工张伟分配 FRONTDESK 角色
CALL sp_assign_app_role('front_zhang', 'FRONTDESK', 'dev_zhou');

-- 给前台主管李婷分配 MANAGER 角色
CALL sp_assign_app_role('front_li', 'MANAGER', 'dev_zhou');

-- 给财务赵敏分配 FINANCE 角色
CALL sp_assign_app_role('acct_zhao', 'FINANCE', 'dev_zhou');

-- 给维修工陈磊分配 MAINTENANCE 角色
CALL sp_assign_app_role('maint_chen', 'MAINTENANCE', 'dev_zhou');

-- 给开发组周超分配 ADMIN 角色
CALL sp_assign_app_role('dev_zhou', 'ADMIN', 'dev_zhou');

-- -- 收回张伟的 FRONTDESK 角色
-- CALL sp_revoke_app_role('front_zhang', 'FRONTDESK');

-- -- 收回开发组周超的 ADMIN 角色
-- CALL sp_revoke_app_role('dev_zhou', 'ADMIN');
