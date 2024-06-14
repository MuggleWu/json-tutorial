#include "leptjson.h"
#include <assert.h>  /* assert() */
#include <stdlib.h>  /* NULL */

/**
 * 宏，用于判断json字符串的第一个字符是否为期望的字符
 * @param c json字符串
 * @param ch 期望的字符
 */
#define EXPECT(c, ch)       do { assert(*c->json == (ch)); c->json++; } while(0)

/**
 * 结构体，用于存储json字符串
 */
typedef struct {
    const char* json;
} lept_context;

/**
 * 跳过json字符串中的空白字符
 * @param json字符串
 */
static void lept_parse_whitespace(lept_context* c) {
    const char *p = c->json;
    while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')
        p++;
    c->json = p;
}

/**
 * 解析 "null" 类型的json字符串
 * @param c json字符串
 * @param v 类型值
 * @return 返回解析结果
 */
static int lept_parse_null(lept_context* c, lept_value* v) {
    EXPECT(c, 'n'); /* 预期第一个字符是'n'，指针向后移动一个字符 */
    if (c->json[0] != 'u' || c->json[1] != 'l' || c->json[2] != 'l')
        return LEPT_PARSE_INVALID_VALUE;
    c->json += 3;
    v->type = LEPT_NULL;
    return LEPT_PARSE_OK;
}

/**
 * 根据第一个字符，按不同场景解析json字符串
 * @param c json字符串
 * @param v 类型值
 * @return 返回解析结果
 */
static int lept_parse_value(lept_context* c, lept_value* v) {
    switch (*c->json) {
        case 'n':  return lept_parse_null(c, v);
        case '\0': return LEPT_PARSE_EXPECT_VALUE;
        default:   return LEPT_PARSE_INVALID_VALUE;
    }
}

/**
 * 跳过空白后，解析json字符串
 * @param v 数据类型
 * @param json json字符串
 * @return 返回解析结果
 */
int lept_parse(lept_value* v, const char* json) {
    lept_context c;
    assert(v != NULL);
    c.json = json;
    v->type = LEPT_NULL;
    lept_parse_whitespace(&c);
    return lept_parse_value(&c, v);
}
/**
 * @param v 类型值
 * @return 返回结构体中的type
 */
lept_type lept_get_type(const lept_value* v) {
    assert(v != NULL);
    return v->type;
}
