#define TEST_IMPLEMENTATION
#include "test.h"
#include "hashmap.h"






/*
podlыy vorishka tests Divanhyper
*/






#define DBL_EPSILON 2.2204460492503131e-16
// Напишите тест для функции обработки строки с использованием данных из
// реализованной хеш таблицы.
TEST(custom)
{
    TEST_ASSERT(false);
}

//
// Общие юнит-тесты
//

TEST(create_hash_map)
{
    HashMap* map = hash_map_create();

    TEST_ASSERT(hash_map_size(map) == 0);
    TEST_ASSERT(hash_map_capacity(map) > 0);

    hash_map_destroy(&map);
}

TEST(destroy_hash_map)
{
    HashMap* map = NULL;
    hash_map_destroy(NULL);
    hash_map_destroy(&map);

    TEST_IGNORE();
}

TEST(create_hash_map_with)
{
    HashMap* map = hash_map_create_with(10);
    TEST_ASSERT(hash_map_size(map) == 0);
    TEST_ASSERT(hash_map_capacity(map) == 10);

    hash_map_clear(map);
    TEST_ASSERT(hash_map_empty(map));
    TEST_ASSERT(hash_map_capacity(map) != 0);
    hash_map_destroy(&map);
}

TEST(push)
{
    HashMap* map = NULL;
    String c = String("hello world");
    String a = String("Анекдот: устроился медведь на работу и сгорел");
    String b = String("Анекдот: Штирлиц спал как убитый. Пару раз его даже обводили мелом");

    String s1 = String("deque");
    String s2 = String("stack");
    String s3 = String("queue");


    TEST_ASSERT(!hash_map_insert(map, 5, c));

    map = hash_map_create_with(4);
    TEST_ASSERT(hash_map_capacity(map) == 4);

    TEST_ASSERT(hash_map_insert(map, 12, a));
    TEST_ASSERT(hash_map_size(map) == 1);
    TEST_ASSERT(hash_map_first(map) != NULL);
    TEST_ASSERT(hash_map_contains(map, 12));

    TEST_ASSERT(!hash_map_insert(map, 12, b));
    TEST_ASSERT(hash_map_size(map) == 1);

    TEST_ASSERT(hash_map_insert(map, 1, s1));
    TEST_ASSERT(hash_map_insert(map, 2, s2));
    TEST_ASSERT(hash_map_size(map) == 3);
    TEST_ASSERT(hash_map_capacity(map) == 4);
    TEST_ASSERT(hash_map_insert(map, 3, s3));
    TEST_ASSERT(hash_map_capacity(map) == 8);

    hash_map_destroy(&map);
}


TEST(push_or_assign)
{
    HashMap* map = hash_map_create_with(4);
    String a = String("Анекдот: 'Кутузов, у тебя завтра экзамен по вождению, не проспи!' - 'Блин, опять надо город сдавать!'");
    String b = String("Анекдот: Штирлиц готовился к бою. А пришла гёрл");

    TEST_ASSERT(hash_map_insert(map, 12, a));
    TEST_ASSERT(hash_map_size(map) == 1);

    TEST_ASSERT(hash_map_insert_or_assign(map, 12, b));
    TEST_ASSERT(hash_map_size(map) == 1);
    TEST_ASSERT(hash_map_load_factor(map) - 0.25 < DBL_EPSILON);

    hash_map_destroy(&map);
}

TEST(remove_pairs)
{
    HashMap* map = hash_map_create_with(4);

    String a = String("Анекдот: Какой фразой можно закончить инцест у грибов? 'Не спорь с матерью'");
    String b = String("Важно помнить, что рот нам дан не для споров с дурачками, а чтоб кушать вкусные пельмени");
    String c = String("Анекдот: Пришёл мужик к психитру. 'Доктор, я Наполнеон!' - 'Ну-с, ничего удивительного, у меня три палаты таких как вы, Наполеонов' - 'Нет, вы не поняли! Я - торт!'");

    TEST_ASSERT(hash_map_insert(map, 0, a));
    TEST_ASSERT(hash_map_insert(map, 1, b));
    TEST_ASSERT(!hash_map_remove(map, 3));
    TEST_ASSERT(!hash_map_remove(map, 5));

    TEST_ASSERT(hash_map_size(map) == 2);
    TEST_ASSERT(hash_map_remove(map, 1));
    TEST_ASSERT(hash_map_size(map) == 1);

    TEST_ASSERT(hash_map_insert(map, 5, c));
    TEST_ASSERT(hash_map_size(map) == 2);

    hash_map_destroy(&map);
}

TEST(map_first_last) {
    HashMap* map = hash_map_create();

    String a = String("Анекдот: Вы когда-нибудь слышали о ресторане на Венере? Хорошая еда, но плохая атмосфера");
    String b = String("Анекдот: Как называется момент, когда дёргается глаз? Это дискотека века");

    TEST_ASSERT(!hash_map_first(map));
    TEST_ASSERT(!hash_map_last(map));

    TEST_ASSERT(hash_map_insert(map, 5, a));
    TEST_ASSERT(hash_map_contains(map, 5));
    TEST_ASSERT(!hash_map_contains(map, 13));

    TEST_ASSERT(hash_map_first(map));
    TEST_ASSERT(hash_map_first(map)->key == 5);
    TEST_ASSERT(string_compare(&hash_map_first(map)->value, &a));

    TEST_ASSERT(hash_map_last(map));
    TEST_ASSERT(hash_map_last(map)->key == 5);
    TEST_ASSERT(string_compare(&hash_map_last(map)->value, &a));

    TEST_ASSERT(hash_map_insert(map, 13, b));
    TEST_ASSERT(hash_map_last(map)->key == 13);
    TEST_ASSERT(string_compare(&hash_map_last(map)->value, &b));

    hash_map_clear(map);
    TEST_ASSERT(!hash_map_first(map));

    hash_map_destroy(&map);
}

int main(void)
{
    return test_run((const test_fn[]) {
            create_hash_map,
            destroy_hash_map,
            create_hash_map_with,
            push,
            push_or_assign,
            remove_pairs,
            map_first_last,
            custom,
            NULL,
    });
}

/*#define TEST_IMPLEMENTATION
#include "test.h"

#include "hashmap.h"

// Напишите тест для функции обработки строки с использованием данных из
// реализованной хеш таблицы.
TEST(custom)
{
    TEST_ASSERT(false);
}

//
// Общие юнит-тесты
//

TEST(create_hash_map)
{
    HashMap* map = hash_map_create();

    TEST_ASSERT(hash_map_size(map) == 0);
    TEST_ASSERT(hash_map_capacity(map) > 0);

    hash_map_destroy(&map);
}

TEST(destroy_hash_map)
{
    HashMap* map = NULL;
    hash_map_destroy(NULL);
    hash_map_destroy(&map);

    TEST_IGNORE();
}

int main(void)
{
    return test_run((const test_fn[]){
        create_hash_map,
        destroy_hash_map,
        custom,
        NULL,
    });
}
*/
