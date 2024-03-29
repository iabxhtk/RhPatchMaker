#ifndef ITEMWEAPON_H
#define ITEMWEAPON_H
class ItemWeapon
{
    int m_Disable;
    int m_ID;
    char m_Name[50];
    char m_KoreanName[50];
    int m_ItemType;
    int m_Description;
    int m_Weight;
    int m_Price;
    int m_SellPrice;
    int m_EquipLevel;
    int m_Damage;
    int m_DamageEnchant[10];
    int m_AttackSpeed;
    int m_AttackRange;
    int m_Miss;
    int m_Critical;
    char m_ImageName[50];
    int m_AttackAni;
    char m_HUMAN_MAN_EQUIP_MESH[50];
    char m_HUMAN_MAN_EQUIP_MESH_CHANGE[50];
    char m_ELF_MAN_EQUIP_MESH[50];
    char m_HALFELF_MAN_EQUIP_MESH[50];
    char m_DAN_MAN_EQUIP_MESH[50];
    char m_GIANT_MAN_EQUIP_MESH[50];
    char m_DARKELF_MAN_EQUIP_MESH[50];
    char m_HALFLING_MAN_EQUIP_MESH[50];
    char m_DEKAN_MAN_EQUIP_MESH[50];
    char m_DEKAN_MAN_EQUIP_MESH_CHANGE[50];
    char m_HUMAN_WOMAN_EQUIP_MESH[50];
    char m_HUMAN_WOMAN_EQUIP_MESH_CHANGE[50];
    char m_ELF_WOMAN_EQUIP_MESH[50];
    char m_HALFELF_WOMAN_EQUIP_MESH[50];
    char m_DAN_WOMAN_EQUIP_MESH[50];
    char m_GIANT_WOMAN_EQUIP_MESH[50];
    char m_DARKELF_WOMAN_EQUIP_MESH[50];
    char m_HALFLING_WOMAN_EQUIP_MESH[50];
    char m_DEKAN_WOMAN_EQUIP_MESH[50];
    char m_DEKAN_WOMAN_EQUIP_MESH_CHANGE[50];
    int m_ItemEntity;
    char m_szEquipSound[50];
    char m_szDropSound[50];
    int m_LevelEnchantPrice[3];
    int m_LevelEnchantRate[3];
    int m_nOptionType[3];
    int m_nOptionValue[3];
    int m_nAttributeType[6];
    int m_nAttributeValue[6];
    char m_ItemExplain[256];
    char m_szItemLargeImage[50];
    char m_szExplainFileName[50];
    int m_GuildBankFee;
    int m_ProductGrade;
    int m_MaxSocketMask;
    int m_SetItemID;
    int m_Durability;
    bool m_Repair;
    int m_RepairPrice;
    int m_Sprit_Weapon_Level;
    int m_Evolution;
    __int64 m_Growth_Exp;
    int m_Growth_Weapon;
    __int64 m_Material_Exp;
    char m_RUMIR_EQUIP_MESH[50];
    char m_NOIR_EQUIP_MESH[50];
    int m_TranscendenceType;
    int m_GroupType;
};

#endif // ITEMWEAPON_H
