#include <memory>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "collect_tarball_items.h"
#include "collectors/collector.h"

class CollectorMock : public Collector {
public:
  MOCK_METHOD(std::vector<TarballItem>, collect, (), (const, override));
};

class CollectTarballItemsTest : public ::testing::Test {
protected:
  TarballItem item1_{std::filesystem::path{"item1.txt"},
                     std::filesystem::path{"/foo/item1.md"}};
  TarballItem item2_{std::filesystem::path{"item2.txt"},
                     std::vector<char>{'a', 'b', 'c'}};
  TarballItem item3_{std::filesystem::path{"item3.txt"},
                     std::filesystem::path{"/bar/item3.txt"}};
};

TEST_F(CollectTarballItemsTest, noCollectors) {
  ASSERT_TRUE(collectTarballItems({}).empty());
}

TEST_F(CollectTarballItemsTest, EmptyCollector) {
  // create a mock collector that returns an empty list of items
  auto collector = std::make_unique<CollectorMock>();
  EXPECT_CALL(*collector, collect())
      .WillRepeatedly(::testing::Return(std::vector<TarballItem>{}));

  std::vector<std::unique_ptr<Collector>> collectors;
  collectors.push_back(std::move(collector));

  ASSERT_TRUE(collectTarballItems(collectors).empty());
}

TEST_F(CollectTarballItemsTest, SingleCollector) {
  // create a single mock collector that returns two items
  auto collector = std::make_unique<CollectorMock>();
  EXPECT_CALL(*collector, collect())
      .WillRepeatedly(::testing::Return(std::vector{item1_, item2_}));

  std::vector<std::unique_ptr<Collector>> collectors;
  collectors.push_back(std::move(collector));

  auto collected_items = collectTarballItems(collectors);
  ASSERT_TRUE(::testing::Matches(
      ::testing::UnorderedElementsAre(item1_, item2_))(collected_items));
}

TEST_F(CollectTarballItemsTest, MultipleCollectors) {
  auto collector1 = std::make_unique<CollectorMock>();
  EXPECT_CALL(*collector1, collect())
      .WillRepeatedly(::testing::Return(std::vector{item1_, item2_}));
  auto collector2 = std::make_unique<CollectorMock>();
  EXPECT_CALL(*collector2, collect())
      .WillRepeatedly(::testing::Return(std::vector<TarballItem>{}));
  auto collector3 = std::make_unique<CollectorMock>();
  EXPECT_CALL(*collector3, collect())
      .WillRepeatedly(::testing::Return(std::vector{item3_}));

  std::vector<std::unique_ptr<Collector>> collectors;
  collectors.push_back(std::move(collector1));
  collectors.push_back(std::move(collector2));
  collectors.push_back(std::move(collector3));

  auto collected_items = collectTarballItems(collectors);
  ASSERT_TRUE(::testing::Matches(::testing::UnorderedElementsAre(
      item1_, item2_, item3_))(collected_items));
}
